
name := "codevoxels"
root := "src/"
src := root+name

lex := root+"lex"
syn := root+"syn"

types := root+"types"
ast   := root+"ast"
api   := root+"api"

init:
    nix-shell -p simple-http-server llvmPackages_20.clang-tools rocmPackages.llvm.clang-unwrapped emscripten flex bison wabt

gen:
    flex  --header-file={{lex}}.h -o {{lex}}.c {{src}}.l
    bison -o {{syn}}.c -d {{src}}.y 

compile: (gen)
    emcc -o {{src}}.html                       \
        -sFORCE_FILESYSTEM                     \
        -sNO_EXIT_RUNTIME=1                    \
        -sEXPORTED_RUNTIME_METHODS=cwrap,ccall \
        -sEXPORTED_FUNCTIONS=___parse          \
        {{syn}}.c {{lex}}.c {{types}}.c {{ast}}.c {{api}}.c

compile-local: (gen)
    gcc -o {{src}} {{syn}}.c {{lex}}.c {{types}}.c {{ast}}.c {{src}}.c -ly -lfl -g

run: (compile)
    rm ./codevoxels-frontend/public/interpreter/*
    cp {{src}}.js ./codevoxels-frontend/public/interpreter/
    cp {{src}}.wasm ./codevoxels-frontend/public/interpreter/
    pnpm -C ./codevoxels-frontend run dev

run-local: (compile-local)
    ./{{src}} < design/test

debug: (compile-local)
    gdbgui {{src}}

clean:
    rm {{lex}}.c {{lex}}.h {{syn}}.c {{syn}}.h \
    {{lex}}.h.gch {{src}} {{src}}.wasm {{src}}.mjs \
    {{src}}.html {{src}}.js

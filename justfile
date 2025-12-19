
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
    flex  -o {{lex}}.c {{src}}.l  
    bison -o {{syn}}.c -d {{src}}.y 

compile: (gen)
    emcc -o {{src}}.html                 \
        -sFORCE_FILESYSTEM               \
        -sNO_EXIT_RUNTIME=1              \
        -sEXPORTED_RUNTIME_METHODS=cwrap \
        {{syn}}.c {{lex}}.c {{types}}.c {{ast}}.c {{api}}.c

run: (compile)
    simple-http-server .
    firefox localhost:8000/index.html

check: (gen)
    gcc -static -o {{src}} {{syn}}.c {{lex}}.c {{types}}.c {{ast}}.c {{api}}.c -ly -lfl

debug: (check)
    gcc -o {{src}} {{syn}}.c {{lex}}.c {{types}}.c {{ast}}.c -ly -lfl -g
    gdbgui {{src}} -n


clean:
    rm {{lex}}.c {{lex}}.h {{syn}}.c {{syn}}.h {{lex}}.h.gch {{src}} {{src}}.wasm {{src}}.mjs

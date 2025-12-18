
name := "codevoxels"
root := "src/"
src := root+name

lex := root+"lex"
syn := root+"syn"

types := root+"types"
ast   := root+"ast"

init:
    nix-shell -p simple-http-server llvmPackages_20.clang-tools rocmPackages.llvm.clang-unwrapped emscripten flex bison wabt

gen:
    flex  -o {{lex}}.c {{src}}.l  
    bison -o {{syn}}.c -d {{src}}.y 

compile: (gen)
    emcc -o {{src}}.mjs                  \
        -sFORCE_FILESYSTEM               \
        -sNO_EXIT_RUNTIME=1              \
        -sEXPORTED_RUNTIME_METHODS=cwrap \
        {{syn}}.c {{lex}}.c 

run: (compile)
    simple-http-server .
    firefox localhost:8000/index.html

check_appart:
    gcc -o {{src}} {{src}}.c {{types}}.c {{ast}}.c
    ./{{src}}

check: (gen)
    gcc -o {{src}} {{syn}}.c {{lex}}.c {{types}}.c {{ast}}.c -ly -lfl
    ./{{src}}


clean:
    rm {{lex}}.c {{lex}}.h {{syn}}.c {{syn}}.h {{lex}}.h.gch {{src}} {{src}}.wasm {{src}}.mjs

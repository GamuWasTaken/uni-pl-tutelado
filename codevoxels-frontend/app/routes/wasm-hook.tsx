import { useEffect, useState, useCallback } from 'react';

export interface WasmModule {
  ccall: (
    funcName: string,
    returnType: string | null,
    argTypes: string[],
    args: string
  ) => any;
  cwrap: (
    funcName: string,
    returnType: string | null,
    argTypes: string[]
  ) => (args: string) => string
  FS: any;
  [key: string]: any;
}

export function useWasmInterpreter() {
  const [module, setModule] = useState<WasmModule | null>(null);
  const [isLoading, setIsLoading] = useState(true);
  const [error, setError] = useState<Error | null>(null);

  useEffect(() => {
    let isMounted = true;

    const loadWasm = async () => {
      try {
        setIsLoading(true);
        setError(null);

        // Configure the Module object BEFORE loading the script
        // Emscripten expects this to be defined on window
        await new Promise<WasmModule>((resolve, reject) => {
          (window as any).Module = {
            locateFile: (path: string) => {
              if (path.endsWith('.wasm')) {
                return '/interpreter/codevoxels.wasm';
              }
              return path;
            },
            print: (text: string) => {
              console.log('WASM stdout:', text);
            },
            printErr: (text: string) => {
              console.error('WASM stderr:', text);
            },
            onRuntimeInitialized: () => {
              // Module is now fully loaded and ready
              console.log("loaded Wasm!");
              resolve((window as any).Module);
            },
          };

          // Now load the script
          const script = document.createElement('script');
          script.src = '/interpreter/codevoxels.js';
          script.async = true;
          script.onerror = () => reject(new Error('Failed to load WebAssembly module'));
          document.body.appendChild(script);
        }).then((wasmModule) => {
          if (isMounted) {
            setModule(wasmModule);
            setIsLoading(false);
          }
        });
      } catch (err) {
        console.error('Error loading WebAssembly module:', err);
        if (isMounted) {
          setError(err instanceof Error ? err : new Error('Unknown error loading WASM'));
          setIsLoading(false);
        }
      }
    };

    loadWasm();

    return () => {
      isMounted = false;
    };
  }, []);

  const runCode = useCallback(
    (code: string) => {
      console.log("Evaluating code:", code)
      if (!module) {
        console.error('WebAssembly module not loaded yet');
        return null;
      }

      try {
        // Call your interpreter function directly with the code
        // Adjust the function name and parameters based on your WASM exports
        const parse = module.cwrap(
          '__parse',
          'void',
          ['string']
        );

        return parse(code);
      } catch (err) {
        console.error('Error running code:', err);
        throw err;
      }
    },
    [module]
  );

  return {
    module,
    isLoading,
    error,
    runCode,
    isReady: !isLoading && !error && module !== null,
  };
}

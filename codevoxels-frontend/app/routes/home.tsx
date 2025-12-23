import Renderer from "./renderer";
import TopBar from "./topbar";
import CodeEditor from "./code-editor";
import "./home.css"
import { useWasmInterpreter } from "./wasm-hook";

export default function Home() {

  const { module, isLoading, error, runCode, isReady } = useWasmInterpreter();

  if (error) {
    console.error('WASM Error:', error);
  }

  if (isLoading) {
    console.log('Loading WebAssembly interpreter...');
  }

  return  <>

  <TopBar/>
  <div className="home-content-container">
    <div id="editor-window-container">
      <CodeEditor onRunCode={runCode}>
      </CodeEditor>
    </div>
    <div id="renderer-container">
      <Renderer/>
    </div>
  </div>
  </>
}

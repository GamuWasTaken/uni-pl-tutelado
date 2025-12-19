import type { Route } from "./+types/home";
import { Welcome } from "../welcome/welcome";
import { Editor } from "@monaco-editor/react";
import Renderer from "./renderer";
import TopBar from "./topbar";
import CodeEditor from "./code-editor";
import "./home.css"

export function meta({}: Route.MetaArgs) {
  return [
    { title: "codevoxels" },
    { name: "description", content: "Welcome to CodeVoxels!" },
  ];
}

export default function Home() {
  return  <>
  
  <TopBar/>
  <div className="home-content-container">
    <div id="editor-window-container">
      <CodeEditor>
      </CodeEditor>
    </div>
    <div id="renderer-container">
      <Renderer/>
    </div>
  </div>
  </> 
}

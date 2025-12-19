import type { Route } from "./+types/home";
import { Welcome } from "../welcome/welcome";
import { Editor } from "@monaco-editor/react";
import Renderer from "./renderer";
import TopBar from "./topbar";
import CodeEditor from "./code-editor";

export function meta({}: Route.MetaArgs) {
  return [
    { title: "codevoxels" },
    { name: "description", content: "Welcome to CodeVoxels!" },
  ];
}

export default function Home() {
  return  <>
  <TopBar/>
  <Renderer/>
  <CodeEditor>
  </CodeEditor>
  </> 
}

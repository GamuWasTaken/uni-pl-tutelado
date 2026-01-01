import { Editor } from "@monaco-editor/react";
import { useRef } from "react";
import "./code-editor.css"

interface CodeEditorProps {
    onRunCode?: (code: string) => void;
}

export default function CodeEditor({ onRunCode }: CodeEditorProps) {
    const editorRef = useRef<any>(null);

    const handleEditorDidMount = (editor: any) => {
        editorRef.current = editor;
    };

    const handleRunClick = () => {
        if (editorRef.current && onRunCode) {
            const code = editorRef.current.getValue();
            onRunCode(code);
        }
    };

    return <>
    <div className="title-card-container">
        <div className="title-bar">
        <span>Editor</span>
            <button id="run-button" onClick={handleRunClick}><div id="run-button-text">Run!</div></button>
        </div>
            <div className="editor-container">
                <Editor
                        height="100%"
                        defaultLanguage="none"
                        defaultValue="// Start typing..."
                        theme="vs-light"
                        onMount={handleEditorDidMount}
                        />
            </div>
        </div>
    </>
}
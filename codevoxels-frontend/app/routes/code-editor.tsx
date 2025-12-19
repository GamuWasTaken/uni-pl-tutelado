import { Editor } from "@monaco-editor/react";
import "./code-editor.css"

export default function CodeEditor() {

    return <>
    <div className="editor-container">
    <div className="title-card-container">
        <div className="title-bar">
        <span>Editor</span>
        <button id="run-button"><div id="run-button-text">Run!</div></button>
        </div>
            <Editor
                    height="100%"
                    defaultLanguage="javascript"
                    defaultValue="// Start typing..."
                    theme="vs-light"
                    />
        </div>
    </div>
    </>
}
import {

  Links,
  Meta,
  Outlet,
  Scripts,
} from "react-router";

import "./root.css";

export function Layout({ children }: { children: React.ReactNode }) {
  return (
    <html lang="en">
      <head>
        <meta charSet="utf-8" />
        <meta name="viewport" content="" initial-scale="1" />
        <Meta></Meta>
        <Links></Links>
      </head>
      <body>
        {children}
        <Scripts/>
      </body>
    </html>
  );
}

export default function App() {
  return <Outlet />;
}

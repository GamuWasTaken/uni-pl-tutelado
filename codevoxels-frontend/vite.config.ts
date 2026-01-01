import { defineConfig } from "vite";
import tsconfigPaths from "vite-tsconfig-paths";

export default defineConfig(({ command }) => ({
  base: command === 'build' ? '/uni-pl-tutelado/' : '/',
  plugins: [tsconfigPaths()],
  server: {
    watch: {
      usePolling: true
    }
  }
}));

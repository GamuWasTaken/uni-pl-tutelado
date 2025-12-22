import { reactRouter } from "@react-router/dev/vite";
import { defineConfig } from "vite";
import tsconfigPaths from "vite-tsconfig-paths";

export default defineConfig({
  base: '/uni-pl-tutelado/',
  plugins: [reactRouter(), tsconfigPaths()],
  server: {
    watch: {
      usePolling: true
    }
  }
});

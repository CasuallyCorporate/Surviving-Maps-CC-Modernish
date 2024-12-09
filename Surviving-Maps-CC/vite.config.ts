import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'

// https://vite.dev/config/
export default defineConfig({
  plugins: [react()],
  server: {
    proxy: {
      '/motd': {
        target: 'http://localhost:8000/endpoint',
        changeOrigin: true,
        rewrite: (path) => path.replace(/^\/motd/,''),
      }
    }
  }
})

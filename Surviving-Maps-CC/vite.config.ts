import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'

// https://vite.dev/config/
export default defineConfig({
  plugins: [react()],
  server: {
    proxy: {
      '/motd': {
        target: 'http://localhost:8000',
        changeOrigin: true
      },
      '/page': {
        target: 'http://localhost:8000',
        changeOrigin: true
      },
      '/site': {
        target: 'http://localhost:8000',
        changeOrigin: true
      },
      '/coord': {
        target: 'http://localhost:8000',
        changeOrigin: true
      }
    }
  }
})

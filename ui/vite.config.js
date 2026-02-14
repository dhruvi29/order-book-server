import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'

// https://vite.dev/config/
export default defineConfig({
  plugins: [react()],
  // server: {
  //   proxy: {
  //     // Proxy requests starting with '/api'
  //     '/api': {
  //       target: 'http://localhost:8080', // The address of your backend server
  //       changeOrigin: true, // Changes the origin header to the target URL
  //       rewrite: (path) => path.replace('/api', '/'), // Removes the '/api' prefix from the request path
  //     },
  //     '/echo': {
  //       target: 'ws://localhost:8080', // The address of your backend server
  //       changeOrigin: true, // Changes the origin header to the target URL
  //       // rewrite: (path) => path.replace('/api', '/'), // Removes the '/api' prefix from the request path
  //     },
  //   },
  // }
})

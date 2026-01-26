import { useState, useEffect } from 'react'
import './StockPrice.css'

export default function StockPrice() {
  const [stockPrice, setStockPrice] = useState(0)

  useEffect(() => {
    const ws = new WebSocket('ws://127.0.0.1:8080/price')

    ws.onopen = () => {
      console.log('WebSocket connected')
    }

    ws.onmessage = (event) => {
      console.log('Message from server:', event.data)
      setStockPrice(event.data)
    }

    ws.onerror = (error) => {
      console.error('WebSocket error:', error)
    }

    return () => {
      ws.close()
    }
  }, [])

  return (
    <div className="stock-price-container">
      <div className="stock-price-display">
        <span className="stock-price-label">Price:</span>
        <span className="stock-price-value">${parseFloat(stockPrice).toFixed(2)}</span>
      </div>
    </div>
  )
}
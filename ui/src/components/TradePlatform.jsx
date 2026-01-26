import { useState, useEffect } from 'react'

export default function TradePlatform() {

    const [stockPrice, setStockPrice] = useState(0);

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


    }, []);


    return (<div>Price: {stockPrice}</div>);
}
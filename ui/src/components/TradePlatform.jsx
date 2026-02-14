
import { useState } from 'react';
import OrderPutForm from './OrderPutForm';
import StockPrice from './StockPrice';
import OrdersPlacedList from './OrdersPlacedList';
import "../styles/TradePlatform.css"

export default function TradePlatform() {
    const [orders, setOrders] = useState([]);

    const handleOrderSubmit = (order) => {
        setOrders(prev => [...prev, order]);
    };

    return (
        <>
            <StockPrice />
            <div className='main-row'>
                <OrderPutForm onOrderSubmit={handleOrderSubmit} />
                <OrdersPlacedList orders={orders} />
            </div>
        </>
    );
}
import { useState } from 'react';
import '../styles/OrderPutForm.css';

export default function OrderPutForm() {
  const [order, setOrder] = useState({ direction: 'buy', quantity: '', price: '' });
  const [response, setResponse] = useState(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(null);

  const handleChange = (e) => {
    const { name, value } = e.target;
    setOrder((prev) => ({ ...prev, [name]: value }));
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    setLoading(true);
    setError(null);
    setResponse(null);

    // try{
    //     const res = await fetch('/api/order');
    // }catch (err) {
    //     console.log("errorr...")
    // }
    try {
      const res = await fetch('http://localhost:8080/order',{
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(order),
      });
        console.log(res);
        const data = await res.text();
        console.log(data);
      setResponse(data);
    } catch (err) {
      setError('Failed to submit order');
    } finally {
      setLoading(false);
    }
  };

  return (
    <form className="order-form-container" onSubmit={handleSubmit}>
      <h3 className="order-form-title">Submit Order</h3>
      <div className="order-form-fields">
        <div className="order-form-row">
          <label className="order-form-label" htmlFor="direction">Direction</label>
          <select
            className="order-form-select"
            id="direction"
            name="direction"
            value={order.direction}
            onChange={handleChange}
            required
          >
            <option value="buy">Buy</option>
            <option value="sell">Sell</option>
          </select>
        </div>
        <div className="order-form-row">
          <label className="order-form-label" htmlFor="quantity">Quantity</label>
          <input
            className="order-form-input"
            id="quantity"
            name="quantity"
            placeholder="Quantity"
            type="number"
            value={order.quantity}
            onChange={handleChange}
            required
            min={1}
          />
        </div>
        <div className="order-form-row">
          <label className="order-form-label" htmlFor="price">Price</label>
          <input
            className="order-form-input"
            id="price"
            name="price"
            placeholder="Price"
            type="number"
            step="0.01"
            value={order.price}
            onChange={handleChange}
            required
            min={0}
          />
        </div>
      </div>
      <button
        className="order-form-button"
        type="submit"
        disabled={loading}
      >
        {loading ? 'Submitting...' : 'Submit'}
      </button>
      {response && <div className="order-form-response">Response: {JSON.stringify(response)}</div>}
      {error && <div className="order-form-error">{error}</div>}
    </form>
  );
}

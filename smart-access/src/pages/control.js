// Control.js
import React, { useState } from 'react';
import { Checkbox } from 'semantic-ui-react';

function Control() {
    const [isOpen, setIsOpen] = useState(false);

    const handleCheckboxChange = () => {
        // 在這裡處理開關狀態的改變，你可以使用 setIsOpen 來更新狀態
        setIsOpen(!isOpen);
    };

    const getCrawlerData = async () => {
        try {
            // const response = await fetch('http://localhost:3001/'); // 請根據實際後端端口設置
            const response = await fetch('http://smart-access-2316b.web.app/api/switch-status'); // 請根據實際後端端口設置

            const data = await response.json();

            // 從後端獲得的開關狀態更新組件狀態
            setIsOpen(data.status === 1);
        } catch (error) {
            console.error('Error fetching data from backend:', error);
        }
    };

    return (
        <div>
            <h2>東華宿舍大門狀態</h2>
            <Checkbox
                toggle
                label={`開關狀態: ${isOpen ? '開' : '關'}`}
                checked={isOpen}
                onChange={handleCheckboxChange}
            />
        </div>
    );
}

export default Control;

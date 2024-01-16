# Bluefruit interface
C++ interface for the [Adafruit Bluefruit LE UART Friend](https://www.adafruit.com/product/2479) module on IOS, MacOS and Android mobile devices using the [bluefruit-protocoll](https://learn.adafruit.com/bluefruit-le-connect/controller).



## Implementation and basic use

This exampleuses `huart1` for the UART communication with the Bluefruit LE device.

**Main loop**
```cpp
  /* USER CODE BEGIN WHILE */
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, bluefruit.get_rx_buffer(), bluefruit.get_rx_buffer_length());
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
  while (true) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    std::cout << bluefruit << std::endl;
    HAL_Delay(250);
  }
#pragma clang diagnostic pop
  /* USER CODE END 3 */
}
```

**Interrupt**
```cpp
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size) {
  if (huart->Instance != USART2) return;

  std::cout << "Interrupt received: " << size << "bytes" << std::endl;
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
  bluefruit.parse(size);
  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *) bluefruit.get_rx_buffer(), bluefruit.get_rx_buffer_length());
}
``````

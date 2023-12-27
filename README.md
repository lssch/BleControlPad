# Bluefruit interface
C++ interface for the [Adafruit Bluefruit LE UART Friend](https://www.adafruit.com/product/2479) module on IOS, MACOS and Android mobile devices using the [bluefruit-protocoll](https://learn.adafruit.com/bluefruit-le-connect/controller).



## Implementation and basic use

**Main loop**
```cpp
  /* USER CODE BEGIN WHILE */
  HAL_UART_Receive_DMA(&huart1, bluefruit.get_rx_buffer(), bluefruit.get_rx_buffer_length());
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
```

**Interrupt**
```cpp
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart == &huart1) {
    HAL_UART_DMAStop(huart);
    bluefruit.parse();
    HAL_UART_Receive_DMA(huart, bluefruit.get_rx_buffer(), bluefruit.get_rx_buffer_length());
  }
}
``````

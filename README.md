# C++ Code Style Guide

## Naming Conventions

### 1. Variables
- **Local Variables**: Use `snake_case`.
  ```cpp
  int max_value = 100;
  float user_input = 0.0f;
  ```
- **Member Variables (Instance Variables)**: Prefix with `m_`.
  ```cpp
  class Example {
  private:
      int m_count;
      std::string m_name;
  public:
      void set_name(const std::string& name) { m_name = name; }
  };
  ```

### 2. Functions
- **Function Names**: Use `camelCase`.
  ```cpp
  void processInput();
  int calculateSum(int a, int b);
  ```

### 3. Constants
- **Use `SCREAMING_SNAKE_CASE`** for constants.
  ```cpp
  const int MAX_SIZE = 256;
  ```

### 4. Classes
- **Class Names**: Use `PascalCase`.
  ```cpp
  class DataProcessor;
  ```



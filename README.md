# SpeedChip

A lightweight performance monitoring and optimization tool for Python applications.

## Features

- **Function Performance Measurement**: Easily measure execution time of any function using decorators
- **Detailed Statistics**: Get min, max, average, and total execution times
- **Performance Reports**: Generate comprehensive performance reports
- **Easy to Use**: Simple decorator-based API
- **Lightweight**: Minimal overhead and dependencies

## Installation

Simply copy `speedchip.py` to your project directory.

## Quick Start

```python
from speedchip import chip

@chip.measure
def my_function():
    # Your code here
    pass

# Run your function
my_function()

# Get performance report
print(chip.report())
```

## Usage Examples

### Basic Usage

```python
from speedchip import chip

@chip.measure
def calculate_fibonacci(n):
    if n <= 1:
        return n
    return calculate_fibonacci(n - 1) + calculate_fibonacci(n - 2)

# Run the function
result = calculate_fibonacci(10)

# View performance statistics
print(chip.report())
```

### Get Statistics for Specific Function

```python
# Get statistics for a specific function
stats = chip.get_stats("calculate_fibonacci")
print(f"Average execution time: {stats['avg_time']:.6f}s")
print(f"Total calls: {stats['count']}")
```

### Using Custom SpeedChip Instance

```python
from speedchip import SpeedChip

# Create your own instance
my_chip = SpeedChip()

@my_chip.measure
def process_data(data):
    # Your processing logic
    pass
```

### Enable/Disable Monitoring

```python
# Disable monitoring temporarily
chip.disable()

# Your code runs without measurement overhead
my_function()

# Re-enable monitoring
chip.enable()
```

### Clear Metrics

```python
# Clear all collected metrics
chip.clear()
```

## API Reference

### SpeedChip Class

#### Methods

- `measure(func)`: Decorator to measure function execution time
- `get_stats(function_name=None)`: Get performance statistics
- `report()`: Generate a formatted performance report
- `clear()`: Clear all collected metrics
- `enable()`: Enable performance monitoring
- `disable()`: Disable performance monitoring

#### Statistics Dictionary

The `get_stats()` method returns a dictionary with:
- `count`: Number of times the function was called
- `total_time`: Total execution time (seconds)
- `avg_time`: Average execution time (seconds)
- `min_time`: Minimum execution time (seconds)
- `max_time`: Maximum execution time (seconds)

## Example Output

```
SpeedChip Performance Report
==================================================

Function: factorial
  Calls: 2
  Total time: 0.000123s
  Average time: 0.000062s
  Min time: 0.000045s
  Max time: 0.000078s

Function: fibonacci
  Calls: 1
  Total time: 0.000234s
  Average time: 0.000234s
  Min time: 0.000234s
  Max time: 0.000234s
```

## License

MIT License

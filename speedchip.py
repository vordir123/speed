"""
SpeedChip - A lightweight performance monitoring and optimization tool
"""

import time
import functools
from typing import Callable, Any, Dict, List
from dataclasses import dataclass, field
from datetime import datetime


@dataclass
class PerformanceMetric:
    """Stores performance metrics for a function call"""
    function_name: str
    execution_time: float
    timestamp: datetime
    args: tuple = field(default_factory=tuple)
    kwargs: dict = field(default_factory=dict)


class SpeedChip:
    """
    Main SpeedChip class for performance monitoring and optimization.

    Usage:
        chip = SpeedChip()

        @chip.measure
        def my_function():
            # your code here
            pass
    """

    def __init__(self):
        self.metrics: List[PerformanceMetric] = []
        self._enabled = True

    def measure(self, func: Callable) -> Callable:
        """
        Decorator to measure function execution time.

        Args:
            func: The function to measure

        Returns:
            Wrapped function that records performance metrics
        """
        @functools.wraps(func)
        def wrapper(*args, **kwargs) -> Any:
            if not self._enabled:
                return func(*args, **kwargs)

            start_time = time.perf_counter()
            try:
                result = func(*args, **kwargs)
                return result
            finally:
                end_time = time.perf_counter()
                execution_time = end_time - start_time

                metric = PerformanceMetric(
                    function_name=func.__name__,
                    execution_time=execution_time,
                    timestamp=datetime.now(),
                    args=args,
                    kwargs=kwargs
                )
                self.metrics.append(metric)

        return wrapper

    def get_stats(self, function_name: str = None) -> Dict[str, Any]:
        """
        Get statistics for measured functions.

        Args:
            function_name: Optional function name to filter by

        Returns:
            Dictionary containing performance statistics
        """
        filtered_metrics = self.metrics
        if function_name:
            filtered_metrics = [m for m in self.metrics if m.function_name == function_name]

        if not filtered_metrics:
            return {
                "count": 0,
                "total_time": 0,
                "avg_time": 0,
                "min_time": 0,
                "max_time": 0
            }

        times = [m.execution_time for m in filtered_metrics]

        return {
            "count": len(filtered_metrics),
            "total_time": sum(times),
            "avg_time": sum(times) / len(times),
            "min_time": min(times),
            "max_time": max(times)
        }

    def report(self) -> str:
        """
        Generate a performance report.

        Returns:
            Formatted string with performance statistics
        """
        if not self.metrics:
            return "No metrics collected yet."

        function_names = set(m.function_name for m in self.metrics)

        lines = ["SpeedChip Performance Report", "=" * 50, ""]

        for fname in sorted(function_names):
            stats = self.get_stats(fname)
            lines.append(f"Function: {fname}")
            lines.append(f"  Calls: {stats['count']}")
            lines.append(f"  Total time: {stats['total_time']:.6f}s")
            lines.append(f"  Average time: {stats['avg_time']:.6f}s")
            lines.append(f"  Min time: {stats['min_time']:.6f}s")
            lines.append(f"  Max time: {stats['max_time']:.6f}s")
            lines.append("")

        return "\n".join(lines)

    def clear(self):
        """Clear all collected metrics"""
        self.metrics.clear()

    def enable(self):
        """Enable performance monitoring"""
        self._enabled = True

    def disable(self):
        """Disable performance monitoring"""
        self._enabled = False


# Global instance for convenient usage
chip = SpeedChip()


# Example usage
if __name__ == "__main__":
    @chip.measure
    def fibonacci(n: int) -> int:
        """Calculate fibonacci number"""
        if n <= 1:
            return n
        return fibonacci(n - 1) + fibonacci(n - 2)

    @chip.measure
    def factorial(n: int) -> int:
        """Calculate factorial"""
        if n <= 1:
            return 1
        result = 1
        for i in range(2, n + 1):
            result *= i
        return result

    # Run some test functions
    print("Running performance tests...")
    fibonacci(10)
    factorial(100)
    factorial(1000)

    # Print performance report
    print(chip.report())

from collections import Counter
from typing import List, Tuple

class StatisticsCalculator:
    def __init__(self, data: List[int]) -> None:
        self.data = list(data)

    def mean(self) -> float:
        if not self.data:
            return 0.0
        return sum(self.data) / float(len(self.data))

    def median(self) -> float:
        if not self.data:
            return 0.0
        xs = sorted(self.data)
        n = len(xs)
        mid = n // 2
        if n % 2 == 1:
            return float(xs[mid])
        return (xs[mid - 1] + xs[mid]) / 2.0

    def mode(self) -> Tuple[List[int], int]:
        """
        Returns (modes, frequency). Supports multimodal lists.
        """
        if not self.data:
            return ([], 0)
        counts = Counter(self.data)
        max_freq = max(counts.values())
        modes = sorted([x for x, c in counts.items() if c == max_freq])
        return (modes, max_freq)

if __name__ == "__main__":
    # Example usage:
    data = [1, 2, 2, 3, 4]
    calc = StatisticsCalculator(data)
    print(f"Input: {data}")
    print(f"Mean: {calc.mean():.4f}")
    print(f"Median: {calc.median():.4f}")
    modes, freq = calc.mode()
    print(f"Mode(s) (freq={freq}): {modes}")

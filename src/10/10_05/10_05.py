import matplotlib.pyplot as plt
import csv
import os

def visualize_performance():
    algorithm_labels = [
        "RS_ALGO", "JS_ALGO", "PJW_ALGO", "ELF_ALGO",
        "BKDR_ALGO", "SDBM_ALGO", "DJB_ALGO", "DEK_ALGO", "AP_ALGO"
    ]

    input_file = "benchmark_results.csv"

    if not os.path.exists(input_file):
        return

    sample_sizes = []
    collision_metrics = [[] for _ in range(len(algorithm_labels))]

    try:
        with open(input_file, mode='r', encoding='utf-8') as stream:
            reader = csv.DictReader(stream)
            for row in reader:
                sample_sizes.append(int(row['Sample_Size']))
                for label in algorithm_labels:
                    idx = algorithm_labels.index(label)
                    collision_metrics[idx].append(int(row[label]))
    except Exception:
        return

    plt.figure(figsize=(12, 7))
    plt.style.use('seaborn-v0_8-darkgrid')

    for i, label in enumerate(algorithm_labels):
        plt.plot(sample_sizes, collision_metrics[i], marker='o', linestyle='-', label=label, linewidth=2)

    plt.title("Сравнительный анализ коллизий хэш-функций", fontsize=14, fontweight='bold')
    plt.xlabel("Количество входных строк (Sample Size)", fontsize=12)
    plt.ylabel("Зафиксированные коллизии (Conflicts)", fontsize=12)

    plt.grid(True, which='both', linestyle='--', alpha=0.5)
    plt.legend(loc='upper left', frameon=True, shadow=True)

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    visualize_performance()
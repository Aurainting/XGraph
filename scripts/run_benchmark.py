import time

import networkx as nx


def run_benchmark(dir_path: str) -> None:
    """
    Run benchmark by NetworkX

    :param dir_path: Directory path of the benchmark
    :return:
    """
    if "undirected" in dir_path:
        graph = nx.Graph()
    else:
        graph = nx.DiGraph()

    file_name = dir_path[dir_path.rfind("/") + 1 :]

    # Add nodes
    with open(f"{dir_path}/{file_name}.v", "r", encoding="utf-8") as f:
        lines = f.readlines()

    node_load_t = 0
    for line in lines:
        start_t = time.time()

        graph.add_node(line)

        end_t = time.time()
        node_load_t += end_t - start_t

    print(f"Node load time: {node_load_t}s")

    # Add edges
    with open(f"{dir_path}/{file_name}.e", "r", encoding="utf-8") as f:
        lines = f.readlines()

    edge_load_t = 0
    for line in lines:
        split_res = line.split(" ")
        start_t = time.time()

        if len(split_res) == 2:
            graph.add_edge(split_res[0], split_res[1])
        elif len(split_res) == 3:
            graph.add_edge(split_res[0], split_res[1], weight=float(split_res[2]))

        end_t = time.time()
        edge_load_t += end_t - start_t

    print(f"Edge load time: {edge_load_t}s")


if __name__ == "__main__":
    run_benchmark("../benchmark/wiki-Talk")

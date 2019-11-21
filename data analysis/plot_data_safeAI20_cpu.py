#!/usr/bin/python
'''
This file read the data and generate the plots

Author: Tianyi Gu
Date: 09/13/2019
'''

import argparse
import os

import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

stochastic = ['Q-learning', 'SARSA', 'UCT']
deterministic = ['A*', 'RTA*']
all = ['A*', 'RTA*', 'Q-learning', 'Sarsa', 'UCT']
grid_sizes = ['6', '7', '8', '9', '10']

algorithms = {
    "Q-learning": "Q-learning",
    "SARSA": "Sarsa",
    "UCT": "UCT",
    "A-star": "A*",
    "LSS LRTA-star": "RTA*"
}


def makeDifferencePlot(width, height, xAxis, yAxis, dataframe, dodge, hue,
                       orderList, hueOrderList, xLabel, yLabel, outputName,
                       markerList):
    sns.set(rc={
        'figure.figsize': (width, height),
        'font.size': 27,
        'text.color': 'black'
    })
    ax = sns.pointplot(x=xAxis,
                       y=yAxis,
                       hue=hue,
                       order=orderList,
                       hue_order=hueOrderList,
                       data=dataframe,
                       ci=95,
                       errwidth=3,
                       join=False,
                       dodge=dodge,
                       palette="Set2",
                       markers=markerList)
    ax.tick_params(colors='black', labelsize=22)
    ax.set_yscale("log")
    plt.legend(loc='lower center',bbox_to_anchor=(0.5, 1), ncol=5)

    plt.ylabel(yLabel, color='black', fontsize=27)
    plt.xlabel(xLabel, color='black', fontsize=27)

    plt.setp(ax.get_legend().get_texts(), fontsize='27')
    plt.setp(ax.get_legend().get_title(), fontsize='27')
 
    plt.savefig(outputName, bbox_inches="tight", pad_inches=0)

    plt.close()
    plt.clf()
    plt.cla()
    return


def parse_args():
    parser = argparse.ArgumentParser(description='Create Plots')

    parser.add_argument('-d', '--directory')

    return parser.parse_args()


def main():
    args = parse_args()

    result_dict = dict()

    gridSize = []
    algorithm = []
    cputime = []
    for directory in os.listdir(args.directory):
        if os.path.isdir(args.directory + '/' + directory):
            result_dict[directory] = dict()
            for filename in os.listdir(args.directory + '/' + directory):
                if os.path.isfile(args.directory + '/' + directory + '/' +
                                  filename) and filename in grid_sizes:

                    with open(args.directory + '/' + directory + '/' +
                              filename) as f:
                        for line in f.readlines():
                            if not line:
                                continue

                            gridSize.append(filename)
                            algorithm.append(algorithms[directory])

                            d = line.split()
                            if directory in stochastic:
                                cputime.append((float(d[2])/1000.0)/60.0)
                                # cputime.append(float(d[2]))
                            else:
                                cputime.append(float(d[1]))

    df = pd.DataFrame({
        "Grid Size": gridSize,
        "Algorithm": algorithm,
        "CPU Time": cputime
    })

    markers = [
        "o", "v", "s", "<", "p", "h", "^", "D", "X", ">", "o", "v", "s", "<",
        "p", "h", "^", "D", "X", ">"
    ]

    makeDifferencePlot(13, 10, "Grid Size", "CPU Time", df, 0.35,
                       "Algorithm", grid_sizes, deterministic, "Grid Size",
                       "CPU Time (ms)", "deterministic.eps", markers)

    makeDifferencePlot(13, 10, "Grid Size", "CPU Time", df, 0.35,
                       "Algorithm", grid_sizes, all, "Grid Size",
                       "CPU Time (ms)", "all.eps", markers)

    makeDifferencePlot(13, 10, "Grid Size", "CPU Time", df, 0.35,
                       "Algorithm", grid_sizes, stochastic, "Grid Size",
                       "CPU Time (min)", "stochastic.eps", markers)




if __name__ == "__main__":
    main()

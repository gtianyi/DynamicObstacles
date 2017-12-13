import numpy as np
import matplotlib.pyplot as plt
import argparse
import os
import json

stochastic = ['Q-learning', 'SARSA', 'UCT']
deterministic = ['A-star', 'LSS LRTA-star']
grid_sizes = ['6', '7', '8', '9', '10']


def mean(data):
    n = len(data)
    return sum(data)/float(n)


def _ss(data):
    c = mean(data)
    ss = sum((x-c)**2 for x in data)
    return ss


def stddev(data):
    n = len(data)
    ss = _ss(data)
    pvar = ss/n
    return pvar**0.5


def autolabel(rects, ax):
    """
    Attach a text label above each bar displaying its height
    """
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x() + rect.get_width()/2., 1.05*height,
                '%d' % int(height),
                ha='center', va='bottom')


def gen_cpu_plot(data, methods, name, width_step=0.2):
    N = 5
    ind = np.arange(N)  # the x locations for the groups
    width = 0       # the width of the bars

    fig, ax = plt.subplots()

    rect_list = list()
    method_name_list = list()
    for i, method in enumerate(data):
        if method not in methods:
            continue
        print method
        method_name_list.append(method)

        means = list()
        stds = list()
        for size in grid_sizes:
            means.append(data[method][size]['avg'])
            stds.append(data[method][size]['std'])

        print means
        print stds

        rect = ax.bar(ind + width, means, width_step, yerr=stds)
        rect_list.append(rect)
        width += width_step

    # add some text for labels, title and axes ticks
    ax.set_ylabel('CPU Time (ms)')
    ax.set_xlabel('Grid Size')
    ax.set_title(name + ' Methods')
    ax.set_xticks(ind + width / 2)
    ax.set_xticklabels(('6', '7', '8', '9', '10'))

    r_list = list()
    for rect in rect_list:
        # autolabel(rect, ax)
        r_list.append(rect[0])

    ax.legend(tuple(r_list), tuple(method_name_list))

    plt.savefig(name.lower() + '_cpu_time.png')


def gen_sr_plot(data):
    N = 5
    width_step = 0.15
    ind = np.arange(N)  # the x locations for the groups
    width = 0       # the width of the bars

    fig, ax = plt.subplots()

    rect_list = list()
    method_name_list = list()
    for i, method in enumerate(data):
        print method
        method_name_list.append(method)

        srs = list()
        for size in grid_sizes:
            srs.append(data[method][size]['sr'])

        print srs

        rect = ax.bar(ind + width, srs, width_step)
        rect_list.append(rect)
        width += width_step

    # add some text for labels, title and axes ticks
    ax.set_ylabel('Wins/Losses')
    ax.set_xlabel('Grid Size')
    ax.set_title('Success Rate for All Methods')
    ax.set_xticks(ind + width / 2)
    ax.set_xticklabels(('6', '7', '8', '9', '10'))

    r_list = list()
    for rect in rect_list:
        # autolabel(rect, ax)
        r_list.append(rect[0])

    ax.legend(tuple(r_list), tuple(method_name_list))

    plt.savefig('success_rate.png')


def parse_args():
    parser = argparse.ArgumentParser(description='Create Plots')

    parser.add_argument('-d', '--directory')

    return parser.parse_args()


def main():
    args = parse_args()

    result_dict = dict()
    for directory in os.listdir(args.directory):
        if os.path.isdir(args.directory + '/' + directory):
            result_dict[directory] = dict()
            for filename in os.listdir(args.directory + '/' + directory):
                if os.path.isfile(args.directory + '/' + directory + '/' + filename) and filename in grid_sizes:

                    with open(args.directory + '/' + directory + '/' + filename) as f:
                        trials = 0
                        win_sum = 0
                        time_data = list()
                        step_data = list()
                        for line in f.readlines():
                            if not line:
                                continue

                            d = line.split()
                            if d[0] == 'win':
                                win_sum += 1
                            if directory in stochastic:
                                step_data.append(int(d[1]))
                                time_data.append(int(d[2]))
                            else:
                                time_data.append(int(float(d[1])))

                            trials += 1

                        sr = win_sum / float(trials)

                        obj = {
                            'avg': int(mean(time_data)),
                            'std': int(stddev(time_data)),
                            'sr': sr
                        }

                        if directory in stochastic:
                            obj['steps'] = int(mean(step_data))

                        result_dict[directory][filename] = obj

    print json.dumps(result_dict, indent=1)

    gen_cpu_plot(result_dict, deterministic, 'Deterministic', width_step=0.35)
    gen_cpu_plot(result_dict, stochastic, 'Stochastic', width_step=0.2)
    gen_sr_plot(result_dict)


if __name__ == "__main__":
    main()

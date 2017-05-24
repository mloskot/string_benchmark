"""
Plots graphs from Celero benchmark results generated in CSV files.

Author: Mateusz Loskot <mateusz@loskot.net>
License: http://unlicense.org
"""
import csv
import os
import sys
from bokeh.charts import Bar, output_file, show

def read_number(string_value):
    """Reads string as float or int"""
    try:
        return int(string_value)
    except ValueError:
        return float(string_value)

def read_results(csv_file):
    """Reads Celero benchmark measurements into dictionary."""
    assert os.path.isfile(csv_file)
    with open(csv_file) as file:
        data = {}
        reader = csv.reader(file, skipinitialspace=True, quotechar="'")
        header = None
        for i, row in enumerate(reader):
            if i == 0:
                header = row
                assert header[0].lower() == 'group'
                continue

            group_name = row[0]
            if group_name not in data:
                data[group_name] = {'file': os.path.basename(csv_file), 'experiments': {}}

            experiments = data[group_name]['experiments']
            experiment_name = row[1]
            if experiment_name not in experiments:
                experiments[experiment_name] = {}

            experiment = experiments[experiment_name]
            for j, measure_name in enumerate(header[2:], 2):
                measure_name = measure_name.lower()
                if measure_name not in experiment:
                    experiment[measure_name] = []
                series = experiment[measure_name]
                series.append(read_number(row[j]))

            # validate size of data series
            series_sizes = []
            for measure_name in header[2:]:
                measure_name = measure_name.lower()
                series_sizes.append(len(experiment[measure_name]))
            assert series_sizes[1:] == series_sizes[:-1]
        return data

def generate_report(data):
    """Plots graphs with benchmark results."""
    for group_name, group in data.items():

        data = {}
        x_labels = None
        for experiment_name, experiment in group['experiments'].items():
            if not x_labels:
                x_labels = experiment['problem space']
            data[experiment_name] = experiment['baseline']
        plot = Bar(data)

        report_file = 'benchmark_{0}.html'.format(group_name)
        output_file(report_file)
        show(plot)

def main(csv_dir=None, csv_file=None):
    """Process CSV files into slick'n'sweet report with graphs."""
    csv_files = []
    if csv_dir:
        sys.exit('TODO')
    csv_files.append(csv_file)
    for csv_file in csv_files:
        results = read_results(csv_file)
        generate_report(results)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit('Usage: {0} <directory with benchmark .csv files>'.format(__file__))
    if os.path.isdir(sys.argv[1]):
        main(csv_dir=sys.argv[1])
    elif sys.argv[1].endswith('.csv') and os.path.isfile(sys.argv[1]):
        main(csv_file=sys.argv[1])
    else:
        sys.exit('\'{0}\' does not exist'.format(sys.argv[1]))

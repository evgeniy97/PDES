import pandas as pd
import numpy as np
import sys as sys # to get argument from command line
import os.path
import glob # Go throw all files in directory
import re


def calculate_velocity(data, col_name,start):
    """
    data: DataFrame We got information from
    col_name: name of colum we want to calculate velocity
    """
    col_data = data[col_name]
    velocity = [col_data[i] - col_data[i-1] for i in range(1, len(col_data))]
    return np.mean(velocity[start:]), np.sqrt(np.std(velocity) / 1000 )  # Change in future


def get_info(data,col_name,start):
    """
    data: DataFrame We got information from
    col_name: name of colum we want to get
    """
    col_data = data[col_name]
    return col_data[start:]


def check(file_path):
    """
    check if file exits
    if not create it
    file_path: path to the file
    """
    if os.path.exists(file_path):
        return file_path
    else:
        with open(file_path, 'w') as f:
            pass
        return file_path


def save_info(file_path,data):
    """
    file_path: path to the file
    """
    col =['q','value','std']
    df = pd.read_table(file_path,sep=' ',names=col,header=None,float_precision='high')
    # !!!!! Add data to df
    df = df.append(data)
    # Sort DataFrame
    # df = df.sort_values('q')
    # Resave DataFrame
    df.to_csv(file_path, header=None, sep=' ', mode='w+',float_format='%.16f')

def main(start=0):
    """
    start: point we want to start slice
    """
    names =['step','min','max','average','width','utilization']

    for file in glob.glob('result/after_statistics/*.txt'):
        parametrs = re.split('[^.0123456789]',file[:-4]) #  p = [-1] q = [-2]
        # print(file)
        data = pd.read_table(file,sep=' ',names=names,header=None,index_col='step',float_precision='high')
        # Calculate velocity
        velocity_min, velocity_min_std = calculate_velocity(data,'min',start)
        velocity_max, velocity_max_std = calculate_velocity(data,'max',start)
        velocity_average, velocity_average_std = calculate_velocity(data, 'average',start)
        # Just get information from DataFrame
        width, width_std = get_info(data,'width',start)
        utilization, utilization_std = get_info(data, 'utilization',start)
        # Save information
        out_path = 'result/final_' + str(start) '/p' + parametrs[-1]
        save_info(check(out_path + 'min.txt'),[{'q':parametrs[-2], 'value': velocity_min, 'std': velocity_min_std}])
        save_info(check(out_path + 'max.txt'),[{'q':parametrs[-2], 'value': velocity_max, 'std': velocity_max_std}])
        save_info(check(out_path + 'average.txt'),[{'q':parametrs[-2], 'value': velocity_average, 'std': velocity_average_std}])
        save_info(check(out_path + 'width.txt'),[{'q':parametrs[-2], 'value': width, 'std': width_std}])
        save_info(check(out_path + 'utilization.txt'),[{'q':parametrs[-2], 'value': utilization, 'std': utilization_std}])


if __name__ == "__main__":
    main(sys.argv[1]) # sys.argv[0] is a name of the script
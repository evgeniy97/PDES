import pandas as pd
import numpy as np
import os.path
import sys
import glob # Go throw all files in directory
import re


def calculate_velocity(data, col_name):
    """
    data: DataFrame We got information from
    col_name: name of colum we want to calculate velocity
    """
    col_data = data[col_name]
    velocity = [abs(col_data[i] - col_data[i-1]) for i in range(1, len(col_data))]
    return np.mean(velocity), np.std(velocity)


def get_info(data,col_name):
    """
    data: DataFrame We got information from
    col_name: name of colum we want to calculate velocity
    """
    col_data = data[col_name]
    return np.mean(col_data), np.std(col_data)


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
    df = pd.read_table(file_path,sep=' ',names=col,header=None,index_col='q')
    # !!!!! Add data to df
    df = df.append(data)
    # Sort DataFrame
    df = df.sort_values('q')
    # Resave DataFrame
    data.to_csv(file_path, header=None, sep=' ', mode='w')

def main(path):
    """"
    path: path to directory
    """
    names =['step','min','max','average','width','utilization']

    for file in glob.glob('result/*.txt'):
        # !!!! WE NEED TO TAKE q AND p FROM FILE NAME!!!!
        parametrs = re.split('[^.0123456789]','file') #  p = [-1] q = [-2]
        # print(file)
        data = pd.read_table(file,sep=' ',names=names,header=None,index_col='step')
        # Calculate velocity
        velocity_min, velocity_min_std = calculate_velocity(data,'min')
        velocity_max, velocity_max_std = calculate_velocity(data,'max')
        velocity_average, velocity_average_std = calculate_velocity(data, 'average')
        # Just get information from DataFrame
        width, width_std = get_info(data,'width')
        utilization, utilization_std = get_info(data, 'utilization')
        # Save information
        

    pass



if __name__ == "__main__":
    main(sys.argv[1]) # sys.argv[0] is a name of the script
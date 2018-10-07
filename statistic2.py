import pandas as pd
import numpy as np
import sys
import glob # Go throw all files in directory


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


def main(path):
    """"
    path: path to directory
    """
    names =['step','min','max','average','width','utilization'] 

    for file in glob.glob('result/*.txt'):
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
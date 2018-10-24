import pandas as pd
import re
import sys as sys # to get argument from command line

# No error calculate in this version

number_of_simulation = 1000 
number_of_timesteps = 100000 # Unused
def main(path):
    """""
    path: path to directory
    """""
    # Initiliaze colum names
    file_path = path + '/test{}.txt'
    names =['step','min','max','average','width','utilization']
    data = pd.read_table(file_path.format(0),sep=' ',names=names,header=None,index_col='step', float_precision='high')
    for i in range(1, number_of_simulation):
        data += pd.read_table(file_path.format(i),sep=' ',names=names,header=None,index_col='step', float_precision='high')

    data /= number_of_simulation

    # Save data
    # There may be a sence to save output in new directory
    out_path = re.split('/', path)
    out_file_path = out_path[0] + '/after_statistics/'+ out_path[1] + '.txt'
    data.to_csv(out_file_path, header=None, sep=' ', mode='w+', float_format='%.16f')
    # read txt file as data = pd.read_table(FILE_PATH,sep=' ',names=names,header=None,index_col='step')



if __name__ == "__main__":
    main(sys.argv[1]) # sys.argv[0] is a name of the script

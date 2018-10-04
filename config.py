# Tune configuration for N, M, r,p,q there
# for range use npunpy arrange(start,end,step)
# We need it to run large simulation queue

from numpy import arange

N_list = [1024]
M_list = [100000]
r_list = [1.0]
p_list = [.0]
q_list = arange(0.1400,0.1440,0.0002)
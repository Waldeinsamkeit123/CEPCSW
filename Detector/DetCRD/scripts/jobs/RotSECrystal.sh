jj=1
# get procid from command line
procid=$1

# map 0,1,2,...,30 to 1,2,3,...,31
sub_name_number=`expr $procid + $jj`

# format 1,2,3,...,31 to 01,02,03,...,31
sub_name=`printf "%04d\n" $sub_name_number`

# run the real job script by the formatted file name
bash subjob_RotSECrystal_"${sub_name}".sh

#!/bin/bash

RUNDIR=/publicfs/cms/user/wanghan/CEPCSW/runscripts/jobs


	RUN=`printf "%s%s" "Digi_RotSECrystal" ".sh"`
	RUN1=runrc.sh

	echo "/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin/hep_sub -g cms $RUN -argu \"%{ProcId}\" -n 100" >$RUNDIR/$RUN1
#echo "/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin/hep_sub -g cms $RUN -argu \"%{ProcId}\" -n 50" >$RUNDIR/$RUN1
	echo 'jj=1
# get procid from command line
procid=$1

# map 0,1,2,...,30 to 1,2,3,...,31
sub_name_number=`expr $procid + $jj`

# format 1,2,3,...,31 to 01,02,03,...,31
sub_name=`printf "%04d\n" $sub_name_number`

# run the real job script by the formatted file name'>$RUNDIR/$RUN
echo "bash subjob_Digi_RotSECrystal_\"\${sub_name}\".sh">>$RUNDIR/$RUN
chmod u+x $RUNDIR/$RUN1
chmod u+x $RUNDIR/$RUN

cd $RUNDIR
./$RUN1

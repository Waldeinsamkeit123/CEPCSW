#!/bin/bash

Outdir=/publicfs/cms/user/wanghan/CEPCSW/Detector/DetCRD/scripts/jobs
jsim=1
while [ $jsim -le 100 ]
do 
IPTCARD=`printf "%s%04d%s" "subjob_RotSECrystal_" "$jsim" ".sh"`
isim=`printf "%04d" $jsim`
echo "#!/bin/bash" >$Outdir/$IPTCARD
echo "cd /publicfs/cms/user/wanghan/CEPCSW" >>$Outdir/$IPTCARD
echo "source setup.sh" >>$Outdir/$IPTCARD
echo "./run.sh /publicfs/cms/user/wanghan/CEPCSW/Detector/DetCRD/scripts/bashes/Standalone-Sim-RotCrystal_${isim}.py" >>$Outdir/$IPTCARD
echo "generate: Standalone-Sim-RotCrystal_${isim}.py"
chmod u+x $Outdir/$IPTCARD
jsim=`expr $jsim + 1`
done

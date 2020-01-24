#!/bin/sh

# Test settings
BASE=/home/projects/benchmarks/workload
test=149_dgx_pytorch
failmetric=6000.0
#i_latest="nvcr.io/nvidia/pytorch:19.04"
i_latest="nscc/local/pytorch:19.04"
image=${IMAGE:-$i_latest}

dockerarg="${DOCKERARGV:---ipc=host}"
###dockerarg="$dockerarg --lustre"

# run on any host
target=1
# run on host specified on command line
pbsname="$test"
if [ x"$1" != x ] ; then target="host=$1" ; pbsname="${test}_${1}" ; fi

# Environment settings
PATH="${PATH}:/app/pbs/bin:/opt/pbs/bin" ; export PATH

# Script settings
d=`date +%Y/%m/%d/%H/%M/%S`
rundir="$BASE/run/$d/$test"
datadir="$BASE/data/$test"
script=run.pbs
csv="$BASE/results.csv"

tst=""
if [ x"$MULTI" == x ] ; then
 # Only have one test in queue at a time
 qname="${pbsname:0:15}"
 tst=`qstat | awk '{if ( $2 == "'$qname'" ) {print $0}}'`
fi
if [ x"$tst" != x ] ; then exit ; fi

# Prepare and submit test
mkdir -p $rundir && cd $rundir || exit

dataset=/raid/datasets/imagenet/jpeg
archive=/home/projects/ai/datasets/imagenet/raid.datasets.imagenet.jpeg.tar

cat << EOF > run.pbs
#!/bin/bash
#PBS -N $pbsname
#%@FUJITSUSCC:CONTINUOUS_PERFORMANCE_TESTING@%#
#PBS -l select=${target}:ncpus=40:ngpus=8
#PBS -q dgx
#PBS -l walltime=0:30:00
#PBS -j oe
#PBS -P 90000001
set -e
env
nvidia-smi
if [ -d "\$PBS_O_WORKDIR" ] ; then cd "\$PBS_O_WORKDIR" ; fi
if [ x"\$PBS_JOBID" == x ] ; then echo PBS_JOBID unset ; echo FAIL ; exit 1 ; fi
#
dataset="$dataset"
archive="$archive"
if [ ! -d "\$dataset" ] ; then
 echo copy dataset to local SSD
 tar -C / -xf "\$archive" || { rm -rf "\$dataset" ; echo FAIL ; exit 1 ; }
fi
#
image="$image"
nscc-docker run $dockerarg \$image < stdin > stdout.\$PBS_JOBID 2> stderr.\$PBS_JOBID || { echo FAIL ; exit 1 ; }
grep /scratch/.test/README stdout.\$PBS_JOBID || { echo /scratch not mounted ; echo FAIL ; exit 1 ; }
grep /home/.test/README stdout.\$PBS_JOBID || { echo /home not mounted ; echo FAIL ; exit 1 ; }
egrep '^uid=.*groups=.*,110029' stdout.\$PBS_JOBID || { echo group error ; echo FAIL ; exit 1 ; }
#
failmetric="$failmetric"
metric=\`grep '^Train summary' stdout.\$PBS_JOBID | awk '{print \$NF}'\`
echo metric=\"\$metric\"
echo check performance
echo threshold: \$failmetric
echo metric: \$metric
echo \$metric | awk '{if ( \$1 > '\$failmetric' ) {printf "PASS\n"}}' | grep PASS || { echo FAIL ; exit 1 ; }
EOF

cat << EOF > stdin
ls -l /scratch/.test/README /home/.test/README
nvidia-smi
id
cd /workspace/examples/resnet50v1.5
python ./multiproc.py --nproc_per_node 8 ./main.py --arch resnet50 --benchmark-training --fp16 --static-loss-scale 256 $dataset
EOF

jobid=`qsub $QSUB_ARGS run.pbs`
if [ x"$ECHO_JOBID" != x ] ; then echo $jobid ; fi
echo "$test , $jobid , Q , STIME , UNKNOWN , 0 , 00:00:00 , 0.0 , HOSTNAME , $PWD " >> "$csv"

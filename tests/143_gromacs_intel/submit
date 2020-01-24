#!/bin/sh

# Test settings
BASE=/home/projects/benchmarks/workload
test=143_gromacs_intel
testd=090_gromacs_adh_cubic
failmetric=107
etarget=-1.3e+06
margin=1.0e+5

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
datadir="$BASE/data/$testd"
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

cat << EOF > run.pbs
#!/bin/sh
#PBS -N $pbsname
#%@FUJITSU:NSCC:CONTINUOUS_PERFORMANCE_TESTING@%#
#PBS -l select=${target}:ncpus=24:mpiprocs=6:ompthreads=4${SELECT_GPU}:mem=64gb
#PBS -l walltime=0:05:00
#PBS -j oe
#PBS -P 90000001

set -e

start=\`date +%s\`

module load composerxe/2016.1.150
. /app/gromacs/5.1.2/intelcc_intelmpi/bin/GMXRC.bash

if [ -d "\$PBS_O_WORKDIR" ] ; then cd "\$PBS_O_WORKDIR" ; fi
cp -p "$datadir"/* .

inputfile=pme_verlet.tpr
mpirun /app/gromacs/5.1.2/intelcc_intelmpi/bin/mdrun_mpi.exe -nice 0 -pin on -ntomp \$OMP_NUM_THREADS -s "\$inputfile"
rm -f pme_verlet.tpr state.cpt confout.gro ener.edr

end=\`date +%s\`
((metric=end-start))
echo metric=\"\$metric\"

failmetric="$failmetric"
echo metric: \$metric
echo check time within allowable limit of \$failmetric
if [ \$metric -gt \$failmetric ] ; then echo FAIL ; exit 1 ; fi

echo check for correctness
etarget="$etarget"
margin="$margin"
achieved=\`grep --after-context=1 "Conserved En" md.log | tail -1 | awk '{print \$2}'\`
python -c "from sys import exit; passfail='PASS' if abs((\$achieved)-(\$etarget)) < \$margin else 'FAIL' ; print passfail; exit(0) if passfail == 'PASS' else exit(1);"
EOF

jobid=`qsub $QSUB_ARGS run.pbs`
if [ x"$ECHO_JOBID" != x ] ; then echo $jobid ; fi
echo "$test , $jobid , Q , STIME , UNKNOWN , 0 , 00:00:00 , 0.0 , HOSTNAME , $PWD " >> "$csv"

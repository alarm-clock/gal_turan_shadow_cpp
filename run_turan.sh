#!/bin/bash

graphs=("loc-gowalla_edges.edges"  "web-Stanford.mtx")
script_name=run_turan_test.sh

#"loc-gowalla_edges.edges"

> "$script_name"

chmod 744 "$script_name"


for graph in "${graphs[@]}"; do

    cat <<EOF >>"$script_name"
#!/bin/bash
#PBS -N turan_shadow_test_${graph}
#PBS -q default
#PBS -l select=1:ncpus=2:mem=50gb:ngpus=0:scratch_local=1gb
#PBS -l walltime=12:00:00
#PBS -o /storage/brno2/home/xbukas00/gal/outputs/ts_${graph}.o
#PBS -e /storage/brno2/home/xbukas00/gal/outputs/ts_${graph}.e

source /storage/brno2/home/xbukas00/.node_bashrc
CONTAINER=\$BRNO_HOME/images/dgl_mongo.sif
PROJECT_DIR=\$BRNO_HOME/gal
REPO_DIR=\$PROJECT_DIR/gal_turan_shadow_cpp
RUN_SCRIPT=GALprojekt
GRAPH=\$PROJECT_DIR/dsets/${graph}

trap 'clean_scratch' TERM EXIT

cd \$REPO_DIR
mkdir turan_shadow_test_${graph}
cp -r src turan_shadow_test_${graph}
cp Makefile turan_shadow_test_${graph}
cd turan_shadow_test_${graph}


singularity exec \$CONTAINER make
singularity exec \$CONTAINER ./\$RUN_SCRIPT -f \$GRAPH -k 5 -s -t 50000 -r

cd ..
rm -rf turan_shadow_test_${graph}
EOF

    qsub "$script_name"
    > "$script_name"


done

rm $script_name 
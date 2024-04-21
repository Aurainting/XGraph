#!/bin/bash

cd .. && mkdir benchmark

if [ -d "../benchmark" ]; then
  cd ../benchmark || exit
  wget https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/wiki-Talk.tar.zst
  wget https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/dota-league.tar.zst
  wget https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-bfs-directed.tar.zst
  wget https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-bfs-undirected.tar.zst
  wget https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-cdlp-directed.tar.zst
  wget https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-cdlp-undirected.tar.zst
  wget https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-pr-directed.tar.zst
  wget https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-pr-undirected.tar.zst
  wget https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-lcc-directed.tar.zst
  wget https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-lcc-undirected.tar.zst
  wget https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-wcc-directed.tar.zst
  wget https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-wcc-undirected.tar.zst
  wget https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-sssp-directed.tar.zst
  wget https://pub-383410a98aef4cb686f0c7601eddd25f.r2.dev/graphalytics/test-sssp-undirected.tar.zst
fi

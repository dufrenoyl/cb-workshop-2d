rm -Rf ./TravelAppSample/*
rm -Rf ./TravelAppSample-Empty/*

scp -r couchbase@192.168.7.183:/home/couchbase/Projects/Qt/TravelAppSample/* ./TravelAppSample
scp -r couchbase@192.168.7.183:/home/couchbase/Projects/Qt/TravelAppSample-Empty/* ./TravelAppSample-Empty

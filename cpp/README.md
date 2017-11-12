# Couchbase C/C++ Workshop

This page contains details how to use the workshop material.

Please see the file [VMSETUP](https://github.com/dufrenoyl/cb-workshop-dev/blob/master/VMSETUP.md) for further details!

## Exercises

### Day 2: Using the Couchbase C Client Library

The starting point for the day 2 execises is the 'TravelAppSample-Empty' application. This is basically an application skeleton which has:

* All the header files
* An empty implementation of the required methods
* Test/demo cases

The application 'TravelAppSample' then contains all the exercise solutions.


| #               | Title                                  | Content                                      | 
| --------------- | -------------------------------------- | -------------------------------------------- |
| 7               | Managing Connections                   | Download and install libcouchbase | 
|                 |                                        | Implement the Connect method in CBDataSource |
|                 |                                        | Implement the CBDataSourceFactory |
| 8               | Create/Update a Document               | Implement the Upsert method in CBDataSource |
| 9               | Get a Document                         | Implement the Get method in CBDataSource |
|                 |                                        | Implement the GetJsonObject method in CBDataSource |
| 10              | Delete a Document                      | Implement the Delete method in CBDataSource |
| 11              | Perform a Multi-Get                    | Implement the MultiGet method in CBDataSource |
| 12              | Query a View                           | Create a View via the UI |
|                 |                                        | Implement the QueryView method in CBDataSource |
| 13              | Query via N1QL                         | Inspect the Global Secondary Indexes |
|                 |                                        | Implement the QueryN1ql method in CBDataSource |
| 14              | A Sample Application                   | Run the Qt Travel-Sample application |
|                 |                                        | Perform a flight search from 'LAX' to 'LHR' |

## Qt Travel-Sample Application

![alt tag](https://raw.githubusercontent.com/dmaier-couchbase/cb-workshop-cpp/master/resources/screenshots/Log-in.png)
![alt tag](https://raw.githubusercontent.com/dmaier-couchbase/cb-workshop-cpp/master/resources/screenshots/Search.png)
![alt tag](https://raw.githubusercontent.com/dmaier-couchbase/cb-workshop-cpp/master/resources/screenshots/Available-Flights.png)
![alt tag](https://raw.githubusercontent.com/dmaier-couchbase/cb-workshop-cpp/master/resources/screenshots/Shopping-Cart.png)


## Help

* Where can I find the View for 'airports/by_name'?

The View code can be found in the 'resources/views' sub-folder. You need to create a Design Document called 'airports' in your 'travel-sample' bucket. Then a View 'by_name' needs to created in this Design Document. You should test the View via the UI before doing the related exercise. 

* How to install the Sample Data?

In case that you didn't install the sample data together with Couchbase 4.x the following shows how to load it:

```
cbdocloader -u couchbase -p couchbase -n 127.0.0.1:8091 -b travel-sample -s 128 travel-sample.zip
```

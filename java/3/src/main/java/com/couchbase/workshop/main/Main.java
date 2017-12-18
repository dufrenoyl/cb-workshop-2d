 /*
  * Copyright 2015 Couchbase, Inc.
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *      http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  */

package com.couchbase.workshop.main;

import com.couchbase.client.java.AsyncBucket;
import com.couchbase.client.java.Bucket;
import com.couchbase.client.java.query.Index;
import static com.couchbase.client.java.query.dsl.Expression.i;
import com.couchbase.workshop.pojo.User;
import com.couchbase.workshop.conn.BucketFactory;
import com.couchbase.workshop.dao.CompanyDao;
import com.couchbase.workshop.dao.DAOFactory;
import com.couchbase.workshop.dao.UserDao;
import com.couchbase.workshop.pojo.Company;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import rx.Observable;

/**
 *
 * @author David Maier <david.maier at couchbase.com>
 */
public class Main {

    public static final Logger LOG = Logger.getLogger(Main.class.getName());

    /**
     * Main entry point of the application
     * 
     * @param args
     * @throws Exception 
     */
    public static void main(String[] args) throws Exception {
        
        
        demoConnect();
        
        //demoCreateUsers();
        //demoCreateCompany();
        //demoAddUserToComp();
        //demoGetComp();
        //demoQueryUserByDate();
        //demoCreateIndexes();
        //demoQueryWithN1QLSimple();
        //demoQueryWithN1QLJoin();
        //demoQueryWithFTS();
        
        //Wait because the results are returned async.
        Thread.sleep(60000);
    }

    /**
     * (1) Establishes a connection by creating an async bucket via the 
     * previously implemented BucketFactory
     * (2) Logs out the bucket name
     */
    private static void demoConnect()
    {
        AsyncBucket bucket = BucketFactory.getAsyncBucket();
        LOG.log(Level.INFO, "bucket = {0}", bucket.name());
        
    }
      
    /**
     * (1) Creates a list of Users
     * (2) Creates a new Observable from the User list
     * (3) Transforms the User to a UserDAO
     * (4) Persists all users by using the UserDAO
     * (5) Logs the result or the error (when an exception occourred)
     * 
     */
    private static void demoCreateUsers() {
        
        
        LOG.info("DEMO - Create user");

        //Create a user object
        List<User> users = new ArrayList<>();

        User dmaier = new User("dmaier", "David", "Maier", "david.maier@couchbase.com", new Date());
        User mmustermann = new User("mmustermann", "Max", "Mustermann", "max.mustermann@mm.de", new Date());

        users.add(dmaier);
        users.add(mmustermann);
        //...

        
        Observable.from(users)
                .map(
                    // Same as: u -> DAOFactory.createUserDao(u)
                    DAOFactory::createUserDao
                )
                .flatMap(
          
                  u -> u.persist()
                    
                )
                .subscribe(
                    u -> LOG.log(Level.INFO, "Wrote user {0}", u.getUid()),
                    e -> LOG.log(Level.SEVERE, "Could not write the user!: {0}", e.toString())
                );
                
                /* FYI: As blocking variant
                .doOnNext(u -> LOG.log(Level.INFO, "Wrote user {0}", u.getUid()))
                .doOnError(e -> LOG.log(Level.SEVERE, "Could not write the user!: {0}", e.toString()))
                .toBlocking()
                .last();
                */
                   
    }

    /**
     * (1) Persists a company by using a CompanyDAO
     * (2) Logs the result or the error (when an exception occourred)
     */
    private static void demoCreateCompany() {
        LOG.info("DEMO - Create company");

        Company comp = new Company("couchbase", "Couchbase Ltd.", "Couchbase Ltd. Address");
        DAOFactory.createCompanyDao(comp).persist().subscribe(
                c -> LOG.log(Level.INFO, "Wrote company {0}", c.getId()),
                e -> LOG.log(Level.SEVERE, "Could not write the company!: {0}", e.toString())
        );

    }

    /**
     * (1) Gets a company by using a CompanyDAO
     * (2) Then adds some users to the returned company
     * (3) Persists the modified company by using a CompanyDAO
     * (4) Logs the result or the error (when an exception occoured)
     */
    private static void demoAddUserToComp() {
        LOG.info("DEMO - Add user to company");

        Company comp = new Company("couchbase");
        CompanyDao compDao = DAOFactory.createCompanyDao(comp);

        compDao.get()
                .map(
                        c -> {
                            
                            List<User> users = new ArrayList<>();
                            users.add(new User("dmaier", "David", "Maier", "david.new@couchbase.com", new Date()));
                            users.add(new User("mnitschinger", "Michael", "Nitschinger", "michael.nitschinger@couchbase.com", new Date()));
                            c.setUsers(users);

                            return c;
                        })
                .flatMap(
                       c -> DAOFactory.createCompanyDao(c).persist())
                .subscribe(
                        c -> LOG.log(Level.INFO, "Wrote company {0}", c.getId()),
                        e -> LOG.log(Level.SEVERE, "Could not write the company!: {0}", e.toString())
                );
    }
    
    /**
     * (1) Create a Company DAO in order to get a specific company
     * (2) Logs the result or the error (when an exception occoured)
     */
    private static void demoGetComp()
    {
        Company cb = new Company("couchbase");
        DAOFactory.createCompanyDao(cb).get().subscribe(
        
                 c -> LOG.log(Level.INFO, "Got company {0}", c.getName()),
                 e -> LOG.log(Level.SEVERE, "Could not get the company!: {0}", e.toString())
        );
        
    }
    
    /**
     * (1) Create a start and end date to query for
     * (2) Query by birthday
     * (3) Logs the result or the error (when an exception occoured)
     * 
     * @throws ParseException 
     */
    private static void demoQueryUserByDate() throws ParseException  
    {
        SimpleDateFormat sdf = new SimpleDateFormat("dd/MM/yyyy");
        Date start = sdf.parse("01/01/2015");
        Date end = sdf.parse("31/12/2020");
        
        UserDao.queryByBirthDay(start, end)
                .subscribe(
                    u -> LOG.log(Level.INFO, "Got user {0}", u.getFirstName()),
                    e -> LOG.log(Level.SEVERE, "Could not query for users!: {0}", e.toString())
        );
    }
    
    /**
     * (1) Create the user indexes
     * (2) Create the company indexes
     */
    private static void demoCreateIndexes()
    {
        Bucket bucket =  BucketFactory.getBucket();
        
        //Primary index is necessary at all
        boolean piCreated = bucket.query(Index.createPrimaryIndex().on(bucket.name()))
                .parseSuccess();
        
        LOG.log(Level.INFO, "Primary Index created = {0}", piCreated);
        
        
       boolean tiCreated = bucket.query(Index.createIndex("by_type").on(bucket.name(), 
                i("type")))
                .parseSuccess();
        
        LOG.log(Level.INFO, "Type Index created = {0}", tiCreated);
        
        LOG.log(Level.INFO, "User Indexes created = {0}", UserDao.createIndexes());
    }
    
    /**
     * (1) Query by name
     * (2) Logs the result or the error (when an exception occoured)
     */
    private static void demoQueryWithN1QLSimple() 
    {
        //Explain: defaultIfEmpty(null)!
        
        UserDao.queryByName("Maier")
                .subscribe(
                    u -> LOG.log(Level.INFO, "Got user {0}", u.getFirstName()),
                    e -> LOG.log(Level.SEVERE, "Could not query for users!: {0}", e.toString())
        );
    }
    
    /**
     * (1) Query by name via the company
     * (2) Logs the result or the error (when an exception occured)
     */
    private static void demoQueryWithN1QLJoin() 
    {        
        CompanyDao.queryUsersByName("couchbase", "Maier")
                .subscribe(
                    u -> LOG.log(Level.INFO, "Got user {0}", u.getFirstName()),
                    e -> LOG.log(Level.SEVERE, "Could not query for users!: {0}", e.toString())
        );
    }
    
    /**
     * (1) Query by name via the company
     * (2) Logs the result or the error (when an exception occured)
     */
    private static void demoQueryWithFTS() 
    {        
        CompanyDao.queryFTS("user email string")
                .subscribe(
                    u -> LOG.log(Level.INFO, "Got user {0}", u.getFirstName()),
                    e -> LOG.log(Level.SEVERE, "Could not query for users!: {0}", e.toString())
        );
    }
}

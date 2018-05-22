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

 package com.couchbase.workshop.dao;

 import com.couchbase.client.java.AsyncBucket;
 import com.couchbase.client.java.document.JsonDocument;
 import com.couchbase.client.java.document.json.JsonArray;
 import com.couchbase.client.java.document.json.JsonObject;
 import com.couchbase.client.java.query.N1qlQuery;
 import com.couchbase.client.java.search.SearchQuery;
 import com.couchbase.client.java.search.queries.MatchPhraseQuery;
 import com.couchbase.workshop.conn.BucketFactory;
 import com.couchbase.workshop.pojo.Company;
 import com.couchbase.workshop.pojo.User;
 import rx.Observable;

 import java.util.ArrayList;
 import java.util.Date;
 import java.util.List;
 import java.util.logging.Level;
 import java.util.logging.Logger;

 /**
  * The Data Access Object which wraps a Company object
  *
  * @author David Maier <david.maier at couchbase.com>
  * @author Raymundo Flores <ray at couchbase.com>
  */
 public class CompanyDao extends AJsonSerializable implements IAsyncDao {

     /**
      * Constants
      */
     public static final String TYPE = "company";
     public static final String PROP_TYPE = "type";
     public static final String PROP_ID = "id";
     public static final String PROP_NAME = "name";
     public static final String PROP_ADDRESS = "address";
     public static final String PROP_USERS = "users";

     /**
      * Logger
      */
     private static final Logger LOG = Logger.getLogger(CompanyDao.class.getName());

     /**
      * Bucket reference
      */
     private final static AsyncBucket bucket = BucketFactory.getAsyncBucket();

     /**
      * Company reference
      */
     private final Company company;

     /**
      * The constructor of the DAO
      *
      * @param company
      */
     public CompanyDao(Company company) {
         this.company = company;
     }

     /**
      * Persist the Company and all the associated Users
      *
      * @return
      */
     @Override
     public Observable<Company> persist() {

         JsonDocument doc = toJson(this.company);

         //Update all users then update the company
         return Observable
                 .from(company.getUsers())
                 .flatMap(u -> DAOFactory.createUserDao(u).persist())
                 .lastOrDefault(null)
                 .flatMap(u -> bucket.upsert(doc))
                 .map(resultDoc -> (Company) fromJson(resultDoc));
     }

     /**
      * To get a company with all it's users
      *
      * @return
      */
     @Override
     public Observable<Company> get() {

         //Get the company by the id
         String id = TYPE + "::" + company.getId();

         return bucket.get(id)
                 .map(resultDoc -> (Company) fromJson(resultDoc))
                 .flatMap(c -> Observable.from(c.getUsers())
                         .flatMap(user -> DAOFactory.createUserDao(user).get()
                                 .doOnNext(u -> {
                                     user.setFirstName(u.getFirstName());
                                     user.setLastName(u.getLastName());
                                     user.setEmail(u.getEmail());
                                     user.setBirthDay(u.getBirthDay());
                                 })
                         )
                         .lastOrDefault(null)
                         .map(u -> c)
                 );
     }


     /**
      * Queries the company uses by filtering by last name
      *
      * @param comp
      * @param lastname
      * @return
      */
     public static Observable<User> queryUsersByName(String comp, String lastname) {
         String query = "SELECT w.{PROP_NAME}, s.* FROM {bucket} w "
                 + "JOIN {bucket} s ON KEYS w.{PROP_USERS} "
                 + "WHERE w.{PROP_TYPE} = '{TYPE}' "
                 + "AND w.{PROP_ID} = '{id}' "
                 + "AND s.{PROP_LASTNAME} = '{lastname}'";

         query = query.replace("{PROP_NAME}", PROP_NAME);
         query = query.replace("{bucket}", bucket.name());
         query = query.replace("{PROP_USERS}", PROP_USERS);
         query = query.replace("{PROP_TYPE}", PROP_TYPE);
         query = query.replace("{TYPE}", TYPE);
         query = query.replace("{PROP_ID}", PROP_ID);
         query = query.replace("{id}", comp);
         query = query.replace("{PROP_LASTNAME}", UserDao.PROP_LASTNAME);
         query = query.replace("{lastname}", lastname);
     
        /*
        FYI: The params in the WHERE clause can be parameterized
        e.g.: WHERE w.type = '$ptype' AND w.id = '$pid' AND 's.lastname' = '$plastname'
        JsonObject placeholderValues = JsonObject.create().put("ptype", "value");
        q = N1qlQuery.parameterized(queryStr_Or_Stmt, placeholderValues);
        */


         return bucket.query(N1qlQuery.simple(query))
                 .flatMap(result -> result.rows())
                 .map(row -> row.value())
                 .map(v -> new User(v.getString(UserDao.PROP_UID), v.getString(UserDao.PROP_FIRSTNAME),
                                 v.getString(UserDao.PROP_LASTNAME), v.getString(UserDao.PROP_EMAIL),
                                 new Date(v.getLong(UserDao.PROP_BDAY))
                         )
                 );

     }

     /**
      * Query user by email.
      *
      * @param email
      * @return
      */
     public static Observable<User> queryFTS(String email) {

         MatchPhraseQuery fts = SearchQuery.matchPhrase(email);

         // Perform the search.

         SearchQuery query = new SearchQuery("idx_default", fts);

         LOG.log(Level.INFO, "Query {0}", query.export().toString());


         return bucket.query(query).flatMap(result -> result.hits())
                 .flatMap(row -> bucket.get(row.id()))
                 .map(doc -> new User(
                         doc.content().getString(UserDao.PROP_UID),
                         doc.content().getString(UserDao.PROP_FIRSTNAME),
                         doc.content().getString(UserDao.PROP_LASTNAME),
                         doc.content().getString(UserDao.PROP_EMAIL),
                         new Date(doc.content().getLong(UserDao.PROP_BDAY))));

     }


     /**
      * Returns the Json object from the given Company
      *
      * @param obj
      * @return
      */
     @Override
     protected JsonDocument toJson(Object obj) {

         Company tmpComp = (Company) obj;

         //Create an empty JSON document
         JsonObject json = JsonObject.empty();

         json.put(PROP_TYPE, TYPE);
         if (tmpComp.getId() != null) {
             json.put(PROP_ID, tmpComp.getId());
         }
         if (tmpComp.getName() != null) {
             json.put(PROP_NAME, tmpComp.getName());
         }
         if (tmpComp.getAddress() != null) {
             json.put(PROP_ADDRESS, tmpComp.getAddress());
         }

         List<User> users = tmpComp.getUsers();

         JsonArray userArray = JsonArray.create();

         for (User user : users) {

             userArray.add(UserDao.TYPE + "::" + user.getUid());
         }

         json.put(PROP_USERS, userArray);

         JsonDocument doc = JsonDocument.create(TYPE + "::" + tmpComp.getId(), json);

         return doc;
     }

     /**
      * Returns the company object from the existing JSON one.
      *
      * Does not yet resolve the User object references completely
      *
      * @param doc
      * @return
      */
     @Override
     protected Object fromJson(JsonDocument doc) {

         JsonObject json = doc.content();

         Company tmpComp = new Company(json.getString(PROP_ID));

         tmpComp.setName(json.getString(PROP_NAME));
         tmpComp.setAddress(json.getString(PROP_ADDRESS));

         List<User> users = new ArrayList<>();
         JsonArray userArr = json.getArray(PROP_USERS);

         for (int i = 0; i < userArr.size(); i++) {

             String userKey = userArr.getString(i);
             String uid = userKey.split(UserDao.TYPE + "::")[1];

             //Don't fill the users yet
             users.add(new User(uid));
         }

         tmpComp.setUsers(users);

         return tmpComp;
     }

 }

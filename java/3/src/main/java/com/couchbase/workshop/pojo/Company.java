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

package com.couchbase.workshop.pojo;

import java.util.ArrayList;
import java.util.List;

/**
 * The Company POJO
 * 
 * @author David Maier <david.maier at couchbase.com>
 */
public class Company {
    
    private String id;
    private String name;
    private String address;
    private List<User> users = new ArrayList<>();

    /**
     * The constructor which only uses the id
     * @param id
     */
    public Company(String id)
    {
        this.id = id;
    }
    
    
    public Company(String id, String name, String address) {
        
        this.id = id;
        this.name = name;
        this.address = address;
    }
    
    /**
     * The full constructor
     * 
     * @param id
     * @param name
     * @param address
     * @param users 
     */
    public Company(String id, String name, String address, List<User> users) {
        
        this(id, name, address);
        
        this.users = users;
    }
    
    //Getters
    public String getId() {
        return id;
    }
    
    public String getName() {
        return name;
    }

    public String getAddress() {
        return address;
    }

    public List<User> getUsers() {
        return users;
    }
    
    //Setters

    /**
     * @param id the id to set
     */
    public void setId(String id) {
        this.id = id;
    }

    /**
     * @param name the name to set
     */
    public void setName(String name) {
        this.name = name;
    }

    /**
     * @param address the address to set
     */
    public void setAddress(String address) {
        this.address = address;
    }

    /**
     * @param users the users to set
     */
    public void setUsers(List<User> users) {
        this.users = users;
    }
    
}

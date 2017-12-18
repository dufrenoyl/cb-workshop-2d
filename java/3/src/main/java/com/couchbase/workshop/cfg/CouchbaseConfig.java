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

package com.couchbase.workshop.cfg;

import java.io.IOException;

/**
 *
 * @author David Maier <david.maier at couchbase.com>
 */
public class CouchbaseConfig extends BaseConfig {

    private static final String FILE_NAME = "cb.properties";
    
    private String[] hosts;
    private int port;
    private String bucket;
    private String username;
    private String password;
    
    
    public CouchbaseConfig() throws IOException {
        super(FILE_NAME);
    }

    public String[] getHosts() {
        
        String propsStr = this.props.getProperty("cb.con.hosts");
        this.hosts = propsStr.split(",");
        return this.hosts;
    }
    
    public int getPort() {
        String portStr = this.props.getProperty("cb.con.port");
        this.port = Integer.parseInt(portStr);
        return this.port;
    }
    
    public String getBucket() {
    
        this.bucket = props.getProperty("cb.con.bucket.name");
        return this.bucket;
    }
    
    public String getUsername() {
        this.username = this.props.getProperty("cb.con.bucket.username"); 
        return this.username;
    }
        
    public String getPassword() {
        this.password = this.props.getProperty("cb.con.bucket.pwd"); 
        return this.password;
    }
    
    public boolean  isQueryEnabled()
    {
        return Boolean.parseBoolean(this.props.getProperty("cb.con.query.enabled"));
    }
           
    
}

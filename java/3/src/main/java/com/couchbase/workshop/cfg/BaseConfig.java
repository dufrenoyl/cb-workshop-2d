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
import java.io.InputStream;
import java.util.Properties;
import java.util.logging.Logger;

/**
 * The basic configuration
 * 
 * @author David Maier <david.maier at couchbase.com>
 */
public class BaseConfig {

    protected final static Logger LOG = Logger.getLogger(BaseConfig.class.getName());


    /**
     * The file to read the properties from
     */
    protected String propsFile;

    /**
     * The related properties object
     */
    protected Properties props;


    /**
     * The constructor which takes the configuration file as argument
     * 
     * @param propsFileName
     * @throws IOException 
     */
    public BaseConfig(String propsFileName) throws IOException
    {
        props  = new Properties();
        InputStream propsStream = this.getClass().getResourceAsStream("/" + propsFileName);

        props.load(propsStream);

    }

    
}

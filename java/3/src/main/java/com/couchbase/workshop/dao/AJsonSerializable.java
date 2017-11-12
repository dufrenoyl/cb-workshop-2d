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

import com.couchbase.client.java.document.JsonDocument;

/**
 * Describes methods to serialize classes
 * 
 * @author David Maier <david.maier at couchbase.com>
 */
public abstract class AJsonSerializable {
    
    /**
     * Serialize to JSON
     * 
     * @param obj
     * @return 
     */
    abstract protected JsonDocument toJson(Object obj);
    
    /**
     * De-serialize from JSON
     * 
     * @param doc 
     * @return  
     */
    abstract protected Object fromJson(JsonDocument doc);
    
}

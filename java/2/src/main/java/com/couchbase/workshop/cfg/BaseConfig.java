
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

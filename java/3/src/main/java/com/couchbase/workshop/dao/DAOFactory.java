package com.couchbase.workshop.dao;

import com.couchbase.workshop.pojo.Company;
import com.couchbase.workshop.pojo.User;
import java.util.Date;

/**
 *
 * @author David Maier <david.maier at couchbase.com>
 */
public class DAOFactory {

    /**
     * Creates an User DAO
     *
     * @param uid
     * @param firstName
     * @param lastName
     * @param email
     * @param bDay
     * @return
     */
    public static UserDao createUserDao(String uid, String firstName, String lastName, String email, Date bDay) {
        User user = new User(uid, firstName, lastName, email, bDay);

        return new UserDao(user);
    }

    /**
     * Creates an User DAO
     *
     * @param uid
     * @return
     */
    public static UserDao createUserDao(String uid) {
       
        User user = new User(uid);

        return new UserDao(user);
    }

    /**
     * Creates an User DAO
     *
     * @param user
     * @return
     */
    public static UserDao createUserDao(User user) {
        return new UserDao(user);
    }

    /**
     * Create a Company DAO
     *
     * @param comp
     * @return
     */
    public static CompanyDao createCompanyDao(Company comp) {
        return new CompanyDao(comp);
    }

}

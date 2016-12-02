/******************************************************************************
 *  * 
 *    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
 *    Source Project Contributors and others.
 *    
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0

 ******************************************************************************/

#ifndef DATE_H_
#define DATE_H_

#include <alljoyn/Status.h>
#include <alljoyn/time/LogModule.h>

namespace ajn {
namespace services {

/**
 * The Date is used in the Clock to set its TimeServiceDateTime
 */
class TimeServiceDate {

  public:

    /**
     * Constructor
     */
    TimeServiceDate();

    /**
     * Destructor
     */
    ~TimeServiceDate();

    /**
     * Initialize the object with its data.
     *
     * @param year Expected: four digit format
     * @param month Expected range: 1-12
     * @param day Expected range: 1-31
     * @return ER_OK if the object was initialized successfully and valid arguments have been passed to this method,
     * otherwise ER_BAD_ARGUMENT status of the appropriate argument is returned
     */
    QStatus init(uint16_t year, uint8_t month, uint8_t day);

    /**
     * Checks whether data of the object is valid, the object variables have a correct values.
     *
     *  - year Expected: four digit format
     *  - month Expected range: 1-12
     *  - day Expected range: 1-31
     * @return TRUE of the object is valid
     */
    bool isValid() const;

    /**
     * Returns year
     *
     * @return Returns year
     */
    const uint16_t getYear() const;

    /**
     * Returns month
     *
     * @return Returns month
     */
    const uint8_t getMonth() const;

    /**
     * Returns day
     *
     * @return Returns day
     */
    const uint8_t getDay() const;

  private:

    /**
     * year Expected: four digit format
     */
    uint16_t m_Year;

    /**
     * month Expected range: 1-12
     */
    uint8_t m_Month;

    /**
     * day Expected range: 1-31
     */
    uint8_t m_Day;

    /**
     * Checks data validity
     *
     * @param year Expected: four digit format
     * @param month Expected range: 1-12
     * @param day Expected range: 1-31
     * @param status ER_OK if all the arguments are valid, or BAD_ARGUMENT status for the bad argument
     */
    bool isValid(uint16_t year, uint8_t month, uint8_t day, QStatus& status) const;
};

} /* namespace services */
} /* namespace ajn */

#endif /* DATE_H_ */
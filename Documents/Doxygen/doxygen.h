
/**
 * @brief Default doxygen comments to inherit from
 */
class CDoxygenCommon
{
public:
  /**
   * @brief Copy value form another object to this.
   *        This object will be a full duplicate.
   * @param oToCopy: Object to copy value from
   * @return Handle to this
   */
  CDoxygenCommon& operator=(const CDoxygenCommon& oToCopy) const;

  /**
   * @brief Add value from another objet with this and generate new.
   * @param oToAdd: Object to add to this.
   * @return Added result of this and oToAdd
   */
  CDoxygenCommon operator+(const CDoxygenCommon& oToAdd) const;

  /**
   * @brief Substract value of another objet from this and generate new.
   * @param oToSub: Object to substract from this.
   * @return Substracted result
   */
  CDoxygenCommon operator-(const CDoxygenCommon& oToSub) const;

  /**
   * @brief Add value to this object
   * @param oToAdd: Object to add
   * @return Handle to this
   */
  CDoxygenCommon& operator+=(const CDoxygenCommon& oToAdd);

  /**
   * @brief Substract value from this object
   * @param oToSub: Object to substract from this
   * @return Handle to this
   */
  CDoxygenCommon& operator-=(const CDoxygenCommon& oToSub);

  /**
   * @brief Compare two items and check if this is lower than object to compare.
   * @param oToCompare: Object to compare to this
   * @return True if this object is lower.
   */
  bool operator<(const CDoxygenCommon& oToCompare) const;

  /**
   * @brief Compare two items and check if this is greater than object to compare.
   * @param oToCompare: Object to compare to this
   * @return True if this object is greater or.
   */
  bool operator>(const CDoxygenCommon& oToCompare) const;

  /**
   * @brief Compare two items and check if this is lower than or equal to object to compare.
   * @param oToCompare: Object to compare to this
   * @return True if this object is lower or equal.
   */
  bool operator<=(const CDoxygenCommon& oToCompare) const;

  /**
   * @brief Compare two items and check if this is greater than or equal to object to compare.
   * @param oToCompare: Object to compare to this
   * @return True if greater object is greater or equal.
   */
  bool operator>=(const CDoxygenCommon& oToCompare) const;

  /**
   * @brief Compare two items and check if this is equal to object to compare.
   * @param oToCompare: Object to compare to this
   * @return True if greater object is equal.
   */
  bool operator==(const CDoxygenCommon& oToCompare) const;

  /**
   * @brief Compare two items and check if this is not equal to object to compare.
   * @param oToCompare: Object to compare to this
   * @return True if greater object is not equal.
   */
  bool operator!=(const CDoxygenCommon& oToCompare) const;

  /**
   * @brief Move value form another object to this.
   *        The source object may not be usable anymore.
   * @param oToMove: Object to move value from
   * @return Handle to this
   */
  CDoxygenCommon& operatorMove(const CDoxygenCommon& oToMove) const;
}


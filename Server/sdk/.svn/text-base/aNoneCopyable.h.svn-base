#ifndef  _aNoneCopyable_H__
#define  _aNoneCopyable_H__

/** 
 * @file aNoneCopyable.h
 * @brief make the class which  used private-inherit can not use the copy-constructor function and '=' operation symbol
 * @author ljc jc.liuworld@hotmail.com
 * @date 2010-05-14
 */

class  aNoneCopyable
{

	protected:
		/** 
		 * @brief  default constructor of this class
		 */

		aNoneCopyable(){};

		/** 
		 * @brief default extract of this class 
		 */

		~aNoneCopyable(){};

	private:

		/** 
		 * @brief copy-constructor  it is for enable;
		 * 
		 */

		aNoneCopyable( const aNoneCopyable& );
		/** 
		 * @brief the  operation symbol of '='
		 * 
		 * @param  
		 */

		const aNoneCopyable & operator = (const aNoneCopyable &);

};

#endif


/*
 * Copyright (C) 2008 Fabien Chereau
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef _STELJSONPARSER_HPP_
#define _STELJSONPARSER_HPP_

#include <QIODevice>
#include <QVariant>
#include <QByteArray>


//! Qt-style iterator over a JSON array. An actual list is not kept in memory,
//! so only forward iteration is supported and all methods, including the constructor,
//! involve read() calls on the QIODevice. Because of this, do not modify the
//! QIODevice between calls to JsonListIterator methods. Also, the toFront()
//! method has a special function and reset() is provided for convenience. Only
//! peekNext() is guaranteed not to modify the QIODevice.
class JsonListIterator
{
public:
	//! Sets up JsonListIterator to read an array. Swallows all whitespace
	//! up to a beginning '[' character. If '[' is not the first non-whitespace
	//! character encountered, reset() is called and an exception is thrown.
	JsonListIterator(QIODevice* input);

	//! Reads and parses the next object from input. Advances QIODevice to
	//! just after the object.
	//! @return the next object from the array
	QVariant next() const;

	//! Reads and parses the next object from input. Does not advance QIODevice.
	//! @return the next object from the array
	QVariant peekNext() const;

	//! Returns true if the next non-whitespace character is not a ']' character.
	bool hasNext();

	//! Seeks QIODevice to the position it was at when the JsonListIterator
	//! constructor was called.
	//! @return QIODevice::seek()'s return value
	bool reset();

	//! Seeks QIODevice to just after the beginning '[' character of the array.
	void toBack();

	//! Seeks QIODevice to just after the terminating ']' character of the
	//! array. If a ']' character is not encountered, something bad will
	//! probably happen.
	void toFront();

private:
	QIODevice* input;
	qint64 startPos;
};

//! @class StelJsonParser
//! Qt-based simple JSON reader inspired by the one from <a href='http://zoolib.sourceforge.net/'>Zoolib</a>.
//! JSON is JavaScript Object Notation. See http://www.json.org/
/*! <p>The mapping with Qt types is done as following:
@verbatim
JSON            Qt
----          -------
null          QVariant::Invalid
object        QVariantMap (QVariant::Map)
array         QVariantList (QVariant::List)
boolean       QVariant::Bool
string        QVariant::String
number        QVariant::Int or QVariant::Double
@endverbatim */
class StelJsonParser
{
public:
	//! Create a JsonListIterator from the given input device.
	static JsonListIterator initListIterator(QIODevice* in) {return JsonListIterator(in);}

	//! Parse the given input stream.
	static QVariant parse(QIODevice* input);
	static QVariant parse(const QByteArray& input);

	//! Serialize the passed QVariant as JSON into the output QIODevice.
	static void write(const QVariant& jsonObject, QIODevice* output, int indentLevel=0);

	//! Serialize the passed QVariant as JSON in a QByteArray.
	static QByteArray write(const QVariant& jsonObject, int indentLevel=0);
};

#endif // _STELJSONPARSER_HPP_

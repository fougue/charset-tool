/****************************************************************************
**
** CharSet Tool
** Copyright FougSys Ltd. (15 Apr. 2014)
** contact@fougsys.fr
**
** This software is a computer program whose purpose is to analyse and convert
** the encoding of text files.
**
** This software is governed by the CeCILL-B license under French law and
** abiding by the rules of distribution of free software.  You can  use,
** modify and/ or redistribute the software under the terms of the CeCILL-B
** license as circulated by CEA, CNRS and INRIA at the following URL
** "http://www.cecill.info".
**
** As a counterpart to the access to the source code and  rights to copy,
** modify and redistribute granted by the license, users are provided only
** with a limited warranty  and the software's author,  the holder of the
** economic rights,  and the successive licensors  have only  limited
** liability.
**
** In this respect, the user's attention is drawn to the risks associated
** with loading,  using,  modifying and/or developing or reproducing the
** software by the user in light of its specific status of free software,
** that may mean  that it is complicated to manipulate,  and  that  also
** therefore means  that it is reserved for developers  and  experienced
** professionals having in-depth computer knowledge. Users are therefore
** encouraged to load and test the software's suitability as regards their
** requirements in conditions enabling the security of their systems and/or
** data to be ensured and,  more generally, to use and operate it in the
** same conditions as regards security.
**
** The fact that you are presently reading this means that you have had
** knowledge of the CeCILL-B license and that you accept its terms.
**
****************************************************************************/

#include "mozilla_universal_charset_detector.h"

#include <QtCore/QString>

#include <fougtools/cpptools/memory_utils.h>

namespace Internal {

static AbstractCharsetDetector::Error toDetectionError(nsresult error)
{
    QString errorMsg;
    if (error == NS_ERROR_OUT_OF_MEMORY)
        errorMsg = QLatin1String("NS_ERROR_OUT_OF_MEMORY");
    return AbstractCharsetDetector::Error(static_cast<int64_t>(error), errorMsg);
}

} // namespace Internal

MozillaUniversalCharsetDetector::MozillaUniversalCharsetDetector(PRUint32 langFilter)
    : nsUniversalDetector(langFilter)
{
}

QByteArray MozillaUniversalCharsetDetector::detectedEncodingName() const
{
    return m_detectedEncodingName;
}

void MozillaUniversalCharsetDetector::init()
{
    this->Reset();
}

bool MozillaUniversalCharsetDetector::handleData(const QByteArray &buffer, Error *error)
{
    const nsresult res = nsUniversalDetector::HandleData(buffer.constData(), buffer.size());
    cpp::checkedAssign(error, Internal::toDetectionError(res));
    return res == NS_OK;
}

void MozillaUniversalCharsetDetector::dataEnd()
{
    nsUniversalDetector::DataEnd();
}

void MozillaUniversalCharsetDetector::Report(const char *charset)
{
    m_detectedEncodingName = charset;
}

void MozillaUniversalCharsetDetector::Reset()
{
    nsUniversalDetector::Reset();
    m_detectedEncodingName.clear();
}

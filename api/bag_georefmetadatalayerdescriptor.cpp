
#include "bag_dataset.h"
#include "bag_georefmetadatalayerdescriptor.h"
#include "bag_metadataprofiles.h"
#include "bag_hdfhelper.h"
#include "bag_private.h"
#include "bag_valuetable.h"

#include <iostream>
#include <array>
#include <H5Cpp.h>


namespace BAG {

//! Constructor.
/*!
\param dataset
    The BAG Dataset this layer belongs to.
\param name
    The name of the simple layer this layer has metadata for.
\param keyType
    The type of the key.
    Must be DT_UINT8, DT_UINT16, DT_UINT32, or DT_UINT64.
\param definition
    The list of fields describing a record/value.
\param chunkSize
    The chunk size the HDF5 DataSet will use.
\param compressionLevel
    The compression level the HDF5 DataSet will use.
*/
GeorefMetadataLayerDescriptor::GeorefMetadataLayerDescriptor(
        Dataset& dataset,
        const std::string& name,
        GeorefMetadataProfile profile,
        DataType keyType,
        RecordDefinition definition,
        uint64_t chunkSize,
        int compressionLevel)
    : LayerDescriptor(dataset.getNextId(), GEOREF_METADATA_PATH + name, name,
                      Georef_Metadata, chunkSize, compressionLevel)
    , m_pBagDataset(dataset.shared_from_this())
    , m_profile(profile)
    , m_keyType(keyType)
    , m_elementSize(Layer::getElementSize(keyType))
    , m_definition(std::move(definition))
{
}

//! Create a georeferenced metadata layer descriptor.
/*!
\param dataset
    The BAG Dataset this layer belongs to.
\param name
    The name of the simple layer this layer has metadata for.
\param keyType
    The type of the key.
    Must be DT_UINT8, DT_UINT16, DT_UINT32, or DT_UINT64.
\param definition
    The list of fields describing a record/value this layer contains for each node.
\param chunkSize
    The chunk size the HDF5 DataSet will use.
\param compressionLevel
    The compression level the HDF5 DataSet will use.

\return
    The new georeferenced metadata layer descriptor.
*/
std::shared_ptr<GeorefMetadataLayerDescriptor> GeorefMetadataLayerDescriptor::create(
            Dataset& dataset,
            const std::string& name,
            GeorefMetadataProfile profile,
            DataType keyType,
            RecordDefinition definition,
            uint64_t chunkSize,
            int compressionLevel)
{
    return std::shared_ptr<GeorefMetadataLayerDescriptor>(
        new GeorefMetadataLayerDescriptor{dataset, name, profile, keyType,
                                          std::move(definition), chunkSize, compressionLevel});
}

//! Open an existing georeferenced metadata layer descriptor.
/*!
\param dataset
    The BAG Dataset this layer belongs to.
\param name
    The name of the simple layer this layer has metadata for.

\return
    The existing georeferenced metadata layer descriptor.
*/
std::shared_ptr<GeorefMetadataLayerDescriptor> GeorefMetadataLayerDescriptor::open(
    Dataset& dataset,
    const std::string& name)
{
    const auto& h5file = dataset.getH5file();

    const std::string internalPath{GEOREF_METADATA_PATH + name + COMPOUND_KEYS};
    const auto h5dataSet = ::H5::DataSet{h5file.openDataSet(internalPath)};

    // Determine keyType.
    const auto fileDataType = h5dataSet.getDataType();

    DataType keyType = DT_UNKNOWN_DATA_TYPE;
    switch(fileDataType.getSize())
    {
    case 1:
        keyType = DT_UINT8;
        break;
    case 2:
        keyType = DT_UINT16;
        break;
    case 4:
        keyType = DT_UINT32;
        break;
    case 8:
        keyType = DT_UINT64;
        break;
    default:
        throw InvalidKeyType{};
    }

    // Determine Record Definition.
    const auto attribute = h5dataSet.openAttribute(COMPOUND_RECORD_DEFINITION);

    const auto fileDataSpace = attribute.getSpace();

    const auto numDims = fileDataSpace.getSimpleExtentNdims();
    if (numDims != 1)
        throw InvalidValueSize{};

    hsize_t numFields = 0;
    fileDataSpace.getSimpleExtentDims(&numFields, nullptr);

    RecordDefinition definition(numFields);

    attribute.read(attribute.getDataType(), definition.data());

    // Determine chunk size and compression level.
    const auto chunkSize = BAG::getChunkSize(h5file, internalPath);
    const auto compressionLevel = BAG::getCompressionLevel(h5file, internalPath);

    // Read metadata profile as string from HDF5 file attribute and convert to GeorefMetadataProfile enum value.
    std::string profileString;
    if (h5dataSet.attrExists(METADATA_PROFILE_TYPE)) {
        const auto metadataProfileAtt = h5dataSet.openAttribute(METADATA_PROFILE_TYPE);
        metadataProfileAtt.read(metadataProfileAtt.getDataType(), profileString);
    } else {
        std::cerr << "Unable to find georeferenced metadata layer attribute '" << METADATA_PROFILE_TYPE <<
                  "' for simple layer '" << name << "', setting profile to '" << UNKOWN_METADATA_PROFILE_NAME <<
                  "'." << std::endl;
        profileString = UNKOWN_METADATA_PROFILE_NAME;
    }

    GeorefMetadataProfile profile;
    auto search = BAG::kStringMapGeorefMetadataProfile.find(profileString);
    if (search != BAG::kStringMapGeorefMetadataProfile.end()) {
        // This is a recognized metadata profile (either a specific known profile or the unknown profile)
        profile = search->second;
    } else {
        std::cerr << "WARNING: Unrecognized georeferenced metadata layer metadata profile '" << profileString <<
                  "' for simple layer '" << name << "', assuming value was '" << UNKOWN_METADATA_PROFILE_NAME <<
                  "'." << std::endl;
        profile = UNKNOWN_METADATA_PROFILE;
    }

    return std::shared_ptr<GeorefMetadataLayerDescriptor>(
        new GeorefMetadataLayerDescriptor{dataset, name, profile, keyType, definition,
                                          chunkSize, compressionLevel});
}


//! \copydoc LayerDescriptor::getDataType
DataType GeorefMetadataLayerDescriptor::getDataTypeProxy() const noexcept
{
    return m_keyType;
}

//! Retrieve the BAG Dataset this layer belongs to.
/*!
\return
    The BAG Dataset.
*/
std::weak_ptr<Dataset> GeorefMetadataLayerDescriptor::getDataset() const &
{
    return m_pBagDataset;
}

//! \copydoc LayerDescriptor::getElementSize
uint8_t GeorefMetadataLayerDescriptor::getElementSizeProxy() const noexcept
{
    return m_elementSize;
}

//! Retrieve the definition of a record/value.
/*!
\return
    The definition of a record/value.
*/
const RecordDefinition&
GeorefMetadataLayerDescriptor::getDefinition() const & noexcept
{
    return m_definition;
}

//! Retrieve the metadata profile type.
/*!
\return
    The metadata profile type.
 */
GeorefMetadataProfile GeorefMetadataLayerDescriptor::getProfile() {
    return m_profile;
}

}  // namespace BAG


#ifndef BAG_DATASET_H
#define BAG_DATASET_H

#include "bag_compounddatatype.h"
#include "bag_georefmetadatalayerdescriptor.h"
#include "bag_config.h"
#include "bag_descriptor.h"
#include "bag_exceptions.h"
#include "bag_fordec.h"
#include "bag_layer.h"
#include "bag_metadata.h"
#include "bag_trackinglist.h"
#include "bag_types.h"
#include "bag_vrtrackinglist.h"

#include <functional>
#include <memory>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>


namespace H5 {

class H5File;

}   //namespace H5

namespace BAG {

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)  // std classes do not have DLL-interface when exporting
#pragma warning(disable: 4275)  // non-DLL-interface class used as base class
#endif

//! The interface for the BAG.
/*!
    This is the BAG Dataset.  It is responsible for creating or reading a BAG.
    It contains the layers and metadata.  It has a descriptor for general
    details.

    It is the only way layers can be created.  It is meant to be the interface
    to the BAG, and answer questions about said BAG.
*/
class BAG_API Dataset final
    : public std::enable_shared_from_this<Dataset>

{
public:
    static std::shared_ptr<Dataset> open(const std::string &fileName,
        OpenMode openMode);

    static std::shared_ptr<Dataset> create(const std::string &fileName,
        Metadata&& metadata, uint64_t chunkSize = 100,
        int compressionLevel = 5);

    void close();

    Dataset(const Dataset&) = delete;
    Dataset(Dataset&&) = delete;

    Dataset& operator=(const Dataset&) = delete;
    Dataset& operator=(Dataset&&) = delete;

    bool operator==(const Dataset &rhs) const noexcept {
        return m_pH5file == rhs.m_pH5file &&
               m_layers == rhs.m_layers &&
               m_pMetadata == rhs.m_pMetadata &&
               m_pTrackingList == rhs.m_pTrackingList &&
               m_descriptor == rhs.m_descriptor &&
               m_pVRTrackingList == rhs.m_pVRTrackingList;
    }

    bool operator!=(const Dataset &rhs) const noexcept {
        return !(rhs == *this);
    }

    Layer& getLayer(uint32_t id) &;
    const Layer& getLayer(uint32_t id) const &;
    std::shared_ptr<Layer> getLayer(LayerType type, const std::string& name) &;
    std::shared_ptr<const Layer> getLayer(LayerType type, const std::string& name) const &;
    std::vector<std::shared_ptr<const Layer>> getLayers() const &;

    std::vector<LayerType> getLayerTypes() const;

    Layer& createSimpleLayer(LayerType type, uint64_t chunkSize,
        int compressionLevel) &;
    GeorefMetadataLayer& createGeorefMetadataLayer(DataType keyType, GeorefMetadataProfile profile,
                                                   const std::string& name, const RecordDefinition& definition,
                                                   uint64_t chunkSize, int compressionLevel) &;
    GeorefMetadataLayer& createGeorefMetadataLayer(GeorefMetadataProfile profile,
                                                   const std::string& name,
                                                   uint64_t chunkSize, int compressionLevel,
                                                   DataType keyType = DT_UINT16) &;
    SurfaceCorrections& createSurfaceCorrections(
        BAG_SURFACE_CORRECTION_TOPOGRAPHY type, uint8_t numCorrectors,
        uint64_t chunkSize, int compressionLevel) &;
    void createVR(uint64_t chunkSize, int compressionLevel, bool makeNode);

    const Metadata& getMetadata() const & noexcept;

    TrackingList& getTrackingList() & noexcept;
    const TrackingList& getTrackingList() const & noexcept;

    std::shared_ptr<GeorefMetadataLayer> getGeorefMetadataLayer(const std::string& name) & noexcept;
    std::shared_ptr<const GeorefMetadataLayer> getGeorefMetadataLayer(const std::string& name) const & noexcept;
    std::vector<std::shared_ptr<GeorefMetadataLayer>> getGeorefMetadataLayers() & noexcept;

    std::shared_ptr<SurfaceCorrections> getSurfaceCorrections() & noexcept;
    std::shared_ptr<const SurfaceCorrections> getSurfaceCorrections() const & noexcept;

    std::shared_ptr<SimpleLayer> getSimpleLayer(LayerType type) & noexcept;
    std::shared_ptr <const SimpleLayer> getSimpleLayer(LayerType type) const & noexcept;

    std::shared_ptr<VRMetadata> getVRMetadata() & noexcept;
    std::shared_ptr<const VRMetadata> getVRMetadata() const & noexcept;

    std::shared_ptr<VRNode> getVRNode() & noexcept;
    std::shared_ptr<const VRNode> getVRNode() const & noexcept;

    std::shared_ptr<VRRefinements> getVRRefinements() & noexcept;
    std::shared_ptr<const VRRefinements> getVRRefinements() const & noexcept;

    std::shared_ptr<VRTrackingList> getVRTrackingList() & noexcept;
    std::shared_ptr<const VRTrackingList> getVRTrackingList() const & noexcept;

    Descriptor& getDescriptor() & noexcept;
    const Descriptor& getDescriptor() const & noexcept;

    std::tuple<double, double> gridToGeo(uint32_t row, uint32_t column) const noexcept;
    std::tuple<uint32_t, uint32_t> geoToGrid(double x, double y) const noexcept;

private:
    Dataset() = default;
    uint32_t getNextId() const noexcept;

    void readDataset(const std::string& fileName, OpenMode openMode);
    void createDataset(const std::string& fileName, Metadata&& metadata,
        uint64_t chunkSize, int compressionLevel);

    std::tuple<bool, float, float> getMinMax(LayerType type,
        const std::string& path = {}) const;

    ::H5::H5File& getH5file() const & noexcept;

    Layer& addLayer(std::shared_ptr<Layer> layer) &;

    //! Custom deleter to not require knowledge of ::H5::H5File destructor here.
    struct BAG_API DeleteH5File final {
        void operator()(::H5::H5File* ptr) noexcept;
    };

    //! The HDF5 file that the BAG is stored in.
    std::unique_ptr<::H5::H5File, DeleteH5File> m_pH5file;
    //! The mandatory and optional layers found in the BAG, including ones
    //! created after opening.
    std::vector<std::shared_ptr<Layer>> m_layers;
    //! The metadata.
    std::unique_ptr<Metadata> m_pMetadata;
    //! The tracking list.
    std::unique_ptr<TrackingList> m_pTrackingList;
    //! The descriptor.
    Descriptor m_descriptor;
    //! The optional VR tracking list.
    std::shared_ptr<VRTrackingList> m_pVRTrackingList;

    friend GeorefMetadataLayer;
    friend GeorefMetadataLayerDescriptor;
    friend InterleavedLegacyLayer;
    friend InterleavedLegacyLayerDescriptor;
    friend LayerDescriptor;
    friend Metadata;
    friend SimpleLayer;
    friend TrackingList;
    friend SimpleLayerDescriptor;
    friend SurfaceCorrections;
    friend SurfaceCorrectionsDescriptor;
    friend ValueTable;
    friend VRMetadata;
    friend VRMetadataDescriptor;
    friend VRNode;
    friend VRNodeDescriptor;
    friend VRRefinements;
    friend VRRefinementsDescriptor;
    friend VRTrackingList;
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

}   //namespace BAG

#endif  //BAG_DATASET_H


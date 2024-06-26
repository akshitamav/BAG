
#include "test_utils.h"
#include <bag_georefmetadatalayer.h>
#include <bag_georefmetadatalayerdescriptor.h>
#include <bag_dataset.h>
#include <bag_metadata.h>
#include <bag_types.h>
#include <bag_valuetable.h>

#include <array>
#include <catch2/catch_all.hpp>
#include <cstring>  //strcmp
#include <string>


using BAG::Dataset;

namespace {

const std::string kMetadataXML{R"(<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<gmi:MI_Metadata xmlns:gmi="http://www.isotc211.org/2005/gmi"
    xmlns:bag="http://www.opennavsurf.org/schema/bag"
    xmlns:gco="http://www.isotc211.org/2005/gco"
    xmlns:gmd="http://www.isotc211.org/2005/gmd"
    xmlns:gml="http://www.opengis.net/gml/3.2"
    xmlns:xlink="http://www.w3.org/1999/xlink"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.opennavsurf.org/schema/bag http://www.opennavsurf.org/schema/bag/bag.xsd">
    <gmd:fileIdentifier>
        <gco:CharacterString>Unique Identifier</gco:CharacterString>
    </gmd:fileIdentifier>
    <gmd:language>
        <gmd:LanguageCode codeList="http://www.loc.gov/standards/iso639-2/" codeListValue="eng">eng</gmd:LanguageCode>
    </gmd:language>
    <gmd:characterSet>
        <gmd:MD_CharacterSetCode codeList="http://www.isotc211.org/2005/resources/Codelist/gmxCodelists.xml#MD_CharacterSetCode" codeListValue="utf8">utf8</gmd:MD_CharacterSetCode>
    </gmd:characterSet>
    <gmd:hierarchyLevel>
        <gmd:MD_ScopeCode codeList="http://www.isotc211.org/2005/resources/Codelist/gmxCodelists.xml#MD_ScopeCode" codeListValue="dataset">dataset</gmd:MD_ScopeCode>
    </gmd:hierarchyLevel>
    <gmd:contact>
        <gmd:CI_ResponsibleParty>
            <gmd:individualName>
                <gco:CharacterString>Name of individual responsible for the BAG</gco:CharacterString>
            </gmd:individualName>
            <gmd:role>
                <gmd:CI_RoleCode codeList="http://www.isotc211.org/2005/resources/Codelist/gmxCodelists.xml#CI_RoleCode" codeListValue="pointOfContact">pointOfContact</gmd:CI_RoleCode>
            </gmd:role>
        </gmd:CI_ResponsibleParty>
    </gmd:contact>
    <gmd:dateStamp>
        <gco:Date>2012-01-27</gco:Date>
    </gmd:dateStamp>
    <gmd:metadataStandardName>
        <gco:CharacterString>ISO 19115</gco:CharacterString>
    </gmd:metadataStandardName>
    <gmd:metadataStandardVersion>
        <gco:CharacterString>2003/Cor.1:2006</gco:CharacterString>
    </gmd:metadataStandardVersion>
    <gmd:spatialRepresentationInfo>
        <gmd:MD_Georectified>
            <gmd:numberOfDimensions>
                <gco:Integer>2</gco:Integer>
            </gmd:numberOfDimensions>
            <gmd:axisDimensionProperties>
                <gmd:MD_Dimension>
                    <gmd:dimensionName>
                        <gmd:MD_DimensionNameTypeCode codeList="http://www.isotc211.org/2005/resources/Codelist/gmxCodelists.xml#MD_DimensionNameTypeCode" codeListValue="row">row</gmd:MD_DimensionNameTypeCode>
                    </gmd:dimensionName>
                    <gmd:dimensionSize>
                        <gco:Integer>100</gco:Integer>
                    </gmd:dimensionSize>
                    <gmd:resolution>
                        <gco:Measure uom="Metres">10</gco:Measure>
                    </gmd:resolution>
                </gmd:MD_Dimension>
            </gmd:axisDimensionProperties>
            <gmd:axisDimensionProperties>
                <gmd:MD_Dimension>
                    <gmd:dimensionName>
                        <gmd:MD_DimensionNameTypeCode codeList="http://www.isotc211.org/2005/resources/Codelist/gmxCodelists.xml#MD_DimensionNameTypeCode" codeListValue="column">column</gmd:MD_DimensionNameTypeCode>
                    </gmd:dimensionName>
                    <gmd:dimensionSize>
                        <gco:Integer>100</gco:Integer>
                    </gmd:dimensionSize>
                    <gmd:resolution>
                        <gco:Measure uom="Metres">10</gco:Measure>
                    </gmd:resolution>
                </gmd:MD_Dimension>
            </gmd:axisDimensionProperties>
            <gmd:cellGeometry>
                <gmd:MD_CellGeometryCode codeList="http://www.isotc211.org/2005/resources/Codelist/gmxCodelists.xml#MD_CellGeometryCode" codeListValue="point">point</gmd:MD_CellGeometryCode>
            </gmd:cellGeometry>
            <gmd:transformationParameterAvailability>
                <gco:Boolean>1</gco:Boolean>
            </gmd:transformationParameterAvailability>
            <gmd:checkPointAvailability>
                <gco:Boolean>0</gco:Boolean>
            </gmd:checkPointAvailability>
            <gmd:cornerPoints>
                <gml:Point gml:id="id1">
                    <gml:coordinates cs="," decimal="." ts=" ">687910.000000,5554620.000000 691590.000000,5562100.000000</gml:coordinates>
                </gml:Point>
            </gmd:cornerPoints>
            <gmd:pointInPixel>
                <gmd:MD_PixelOrientationCode>center</gmd:MD_PixelOrientationCode>
            </gmd:pointInPixel>
        </gmd:MD_Georectified>
    </gmd:spatialRepresentationInfo>
    <gmd:referenceSystemInfo>
        <gmd:MD_ReferenceSystem>
            <gmd:referenceSystemIdentifier>
                <gmd:RS_Identifier>
                    <gmd:code>
                        <gco:CharacterString>PROJCS["UTM-19N-Nad83",
    GEOGCS["unnamed",
        DATUM["North_American_Datum_1983",
            SPHEROID["North_American_Datum_1983",6378137,298.2572201434276],
            TOWGS84[0,0,0,0,0,0,0]],
        PRIMEM["Greenwich",0],
        UNIT["degree",0.0174532925199433],
        EXTENSION["Scaler","0,0,0,0.02,0.02,0.001"],
        EXTENSION["Source","CARIS"]],
    PROJECTION["Transverse_Mercator"],
    PARAMETER["latitude_of_origin",0],
    PARAMETER["central_meridian",-69],
    PARAMETER["scale_factor",0.9996],
    PARAMETER["false_easting",500000],
    PARAMETER["false_northing",0],
    UNIT["metre",1]]</gco:CharacterString>
                    </gmd:code>
                    <gmd:codeSpace>
                        <gco:CharacterString>WKT</gco:CharacterString>
                    </gmd:codeSpace>
                </gmd:RS_Identifier>
            </gmd:referenceSystemIdentifier>
        </gmd:MD_ReferenceSystem>
    </gmd:referenceSystemInfo>
    <gmd:referenceSystemInfo>
        <gmd:MD_ReferenceSystem>
            <gmd:referenceSystemIdentifier>
                <gmd:RS_Identifier>
                    <gmd:code>
                        <gco:CharacterString>VERT_CS["Alicante height",
    VERT_DATUM["Alicante",2000]]</gco:CharacterString>
                    </gmd:code>
                    <gmd:codeSpace>
                        <gco:CharacterString>WKT</gco:CharacterString>
                    </gmd:codeSpace>
                </gmd:RS_Identifier>
            </gmd:referenceSystemIdentifier>
        </gmd:MD_ReferenceSystem>
    </gmd:referenceSystemInfo>
    <gmd:identificationInfo>
        <bag:BAG_DataIdentification>
            <gmd:citation>
                <gmd:CI_Citation>
                    <gmd:title>
                        <gco:CharacterString>Name of dataset input</gco:CharacterString>
                    </gmd:title>
                    <gmd:date>
                        <gmd:CI_Date>
                            <gmd:date>
                                <gco:Date>2008-10-21</gco:Date>
                            </gmd:date>
                            <gmd:dateType>
                                <gmd:CI_DateTypeCode codeList="http://www.isotc211.org/2005/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode" codeListValue="creation">creation</gmd:CI_DateTypeCode>
                            </gmd:dateType>
                        </gmd:CI_Date>
                    </gmd:date>
                    <gmd:citedResponsibleParty>
                        <gmd:CI_ResponsibleParty>
                            <gmd:individualName>
                                <gco:CharacterString>Person responsible for input data</gco:CharacterString>
                            </gmd:individualName>
                            <gmd:role>
                                <gmd:CI_RoleCode codeList="http://www.isotc211.org/2005/resources/Codelist/gmxCodelists.xml#CI_RoleCode" codeListValue="originator">originator</gmd:CI_RoleCode>
                            </gmd:role>
                        </gmd:CI_ResponsibleParty>
                    </gmd:citedResponsibleParty>
                </gmd:CI_Citation>
            </gmd:citation>
            <gmd:abstract>
                <gco:CharacterString>Sample Metadata</gco:CharacterString>
            </gmd:abstract>
            <gmd:status>
                <gmd:MD_ProgressCode codeList="http://www.isotc211.org/2005/resources/Codelist/gmxCodelists.xml#MD_ProgressCode" codeListValue="completed">completed</gmd:MD_ProgressCode>
            </gmd:status>
            <gmd:spatialRepresentationType>
                <gmd:MD_SpatialRepresentationTypeCode codeList="http://www.isotc211.org/2005/resources/Codelist/gmxCodelists.xml#MD_SpatialRepresentationTypeCode" codeListValue="grid">grid</gmd:MD_SpatialRepresentationTypeCode>
            </gmd:spatialRepresentationType>
            <gmd:language>
                <gmd:LanguageCode codeList="http://www.loc.gov/standards/iso639-2/" codeListValue="eng">eng</gmd:LanguageCode>
            </gmd:language>
            <gmd:characterSet>
                <gmd:MD_CharacterSetCode codeList="http://www.isotc211.org/2005/resources/Codelist/gmxCodelists.xml#MD_CharacterSetCode" codeListValue="utf8">utf8</gmd:MD_CharacterSetCode>
            </gmd:characterSet>
            <gmd:topicCategory>
                <gmd:MD_TopicCategoryCode>elevation</gmd:MD_TopicCategoryCode>
            </gmd:topicCategory>
            <gmd:extent>
                <gmd:EX_Extent>
                    <gmd:geographicElement>
                        <gmd:EX_GeographicBoundingBox>
                            <gmd:westBoundLongitude>
                                <gco:Decimal>-66.371629</gco:Decimal>
                            </gmd:westBoundLongitude>
                            <gmd:eastBoundLongitude>
                                <gco:Decimal>-66.316454</gco:Decimal>
                            </gmd:eastBoundLongitude>
                            <gmd:southBoundLatitude>
                                <gco:Decimal>50.114053</gco:Decimal>
                            </gmd:southBoundLatitude>
                            <gmd:northBoundLatitude>
                                <gco:Decimal>50.180077</gco:Decimal>
                            </gmd:northBoundLatitude>
                        </gmd:EX_GeographicBoundingBox>
                    </gmd:geographicElement>
                </gmd:EX_Extent>
            </gmd:extent>
            <bag:verticalUncertaintyType>
                <bag:BAG_VertUncertCode codeList="http://www.opennavsurf.org/schema/bag/bagCodelists.xml#BAG_VertUncertCode" codeListValue="rawStdDev">rawStdDev</bag:BAG_VertUncertCode>
            </bag:verticalUncertaintyType>
            <bag:depthCorrectionType>
                <bag:BAG_DepthCorrectCode codeList="http://www.opennavsurf.org/schema/bag/bagCodelists.xml#BAG_DepthCorrectCode" codeListValue="trueDepth">trueDepth</bag:BAG_DepthCorrectCode>
            </bag:depthCorrectionType>
            <bag:elevationSolutionGroupType>
                <bag:BAG_OptGroupCode codeList="http://www.opennavsurf.org/schema/bag/bagCodelists.xml#BAG_OptGroupCode" codeListValue="cube">cube</bag:BAG_OptGroupCode>
            </bag:elevationSolutionGroupType>
            <bag:nodeGroupType>
                <bag:BAG_OptGroupCode codeList="http://www.opennavsurf.org/schema/bag/bagCodelists.xml#BAG_OptGroupCode" codeListValue="product">product</bag:BAG_OptGroupCode>
            </bag:nodeGroupType>
        </bag:BAG_DataIdentification>
    </gmd:identificationInfo>
    <gmd:dataQualityInfo>
        <gmd:DQ_DataQuality>
            <gmd:scope>
                <gmd:DQ_Scope>
                    <gmd:level>
                        <gmd:MD_ScopeCode codeList="http://www.isotc211.org/2005/resources/Codelist/gmxCodelists.xml#MD_ScopeCode" codeListValue="dataset">dataset</gmd:MD_ScopeCode>
                    </gmd:level>
                </gmd:DQ_Scope>
            </gmd:scope>
            <gmd:lineage>
                <gmd:LI_Lineage>
                    <gmd:processStep>
                        <bag:BAG_ProcessStep>
                            <gmd:description>
                                <gco:CharacterString>List to be determined by WG. I.e. Product Creation</gco:CharacterString>
                            </gmd:description>
                            <gmd:dateTime>
                                <gco:DateTime>2008-10-21T12:21:53</gco:DateTime>
                            </gmd:dateTime>
                            <gmd:processor>
                                <gmd:CI_ResponsibleParty>
                                    <gmd:individualName>
                                        <gco:CharacterString>Name of the processor</gco:CharacterString>
                                    </gmd:individualName>
                                    <gmd:role>
                                        <gmd:CI_RoleCode codeList="http://www.isotc211.org/2005/resources/Codelist/gmxCodelists.xml#CI_RoleCode" codeListValue="processor">processor</gmd:CI_RoleCode>
                                    </gmd:role>
                                </gmd:CI_ResponsibleParty>
                            </gmd:processor>
                            <gmd:source>
                                <gmd:LI_Source>
                                    <gmd:description>
                                        <gco:CharacterString>Source</gco:CharacterString>
                                    </gmd:description>
                                    <gmd:sourceCitation>
                                        <gmd:CI_Citation>
                                            <gmd:title>
                                                <gco:CharacterString>Name of dataset input</gco:CharacterString>
                                            </gmd:title>
                                            <gmd:date>
                                                <gmd:CI_Date>
                                                    <gmd:date gco:nilReason="unknown"/>
                                                    <gmd:dateType>
                                                        <gmd:CI_DateTypeCode codeList="http://www.isotc211.org/2005/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode" codeListValue="creation">creation</gmd:CI_DateTypeCode>
                                                    </gmd:dateType>
                                                </gmd:CI_Date>
                                            </gmd:date>
                                        </gmd:CI_Citation>
                                    </gmd:sourceCitation>
                                </gmd:LI_Source>
                            </gmd:source>
                            <bag:trackingId>
                                <gco:CharacterString>1</gco:CharacterString>
                            </bag:trackingId>
                        </bag:BAG_ProcessStep>
                    </gmd:processStep>
                </gmd:LI_Lineage>
            </gmd:lineage>
        </gmd:DQ_DataQuality>
    </gmd:dataQualityInfo>
    <gmd:metadataConstraints>
        <gmd:MD_LegalConstraints>
            <gmd:useConstraints>
                <gmd:MD_RestrictionCode codeList="http://www.isotc211.org/2005/resources/Codelist/gmxCodelists.xml#MD_RestrictionCode" codeListValue="otherRestrictions">otherRestrictions</gmd:MD_RestrictionCode>
            </gmd:useConstraints>
            <gmd:otherConstraints>
                <gco:CharacterString>some other constraints</gco:CharacterString>
            </gmd:otherConstraints>
        </gmd:MD_LegalConstraints>
    </gmd:metadataConstraints>
    <gmd:metadataConstraints>
        <gmd:MD_SecurityConstraints>
            <gmd:classification>
                <gmd:MD_ClassificationCode codeList="http://www.isotc211.org/2005/resources/Codelist/gmxCodelists.xml#MD_ClassificationCode" codeListValue="unclassified">unclassified</gmd:MD_ClassificationCode>
            </gmd:classification>
            <gmd:userNote>
                <gco:CharacterString>some user node</gco:CharacterString>
            </gmd:userNote>
        </gmd:MD_SecurityConstraints>
    </gmd:metadataConstraints>
</gmi:MI_Metadata>
)"};

}  // namespace

bool operator==(
    const FieldDefinition& lhs,
    const FieldDefinition& rhs)
{
    if (lhs.type != rhs.type)
        return false;

    if (lhs.name == nullptr || rhs.name == nullptr)
        return lhs.name == rhs.name;

    return strcmp(lhs.name, rhs.name) == 0;
}

//  const Records& getRecords() const & noexcept;
//  const RecordDefinition& getDefinition() const & noexcept;
//  size_t getFieldIndex(const std::string& name) const;
//  const char* getFieldName(size_t index) const &;
TEST_CASE("test value table reading empty", "[valuetable][getDefinition][getRecords][getFieldIndex][getFieldName]")
{
    UNSCOPED_INFO("Check dataset was created successfully.");
    const TestUtils::RandomFileGuard tmpFileName;

    BAG::Metadata metadata;
    metadata.loadFromBuffer(kMetadataXML);

    constexpr uint64_t chunkSize = 100;
    constexpr int compressionLevel = 6;

    auto pDataset = Dataset::create(tmpFileName, std::move(metadata),
        chunkSize, compressionLevel);
    REQUIRE(pDataset);

    // Make a new Value Table.
    constexpr BAG::DataType keyType = DT_UINT8;
    const std::string layerName{"elevation"};
    constexpr size_t kExpectedDefinitionSize = 4;

    const char kFieldName0[]{"float value"};
    const char kFieldName1[]{"uint32 value"};
    const char kFieldName2[]{"bool value"};
    const char kFieldName3[]{"string value"};

    BAG::RecordDefinition kExpectredDefinition(4);
    kExpectredDefinition[0].name = kFieldName0;
    kExpectredDefinition[0].type = DT_FLOAT32;
    kExpectredDefinition[1].name = kFieldName1;
    kExpectredDefinition[1].type = DT_UINT32;
    kExpectredDefinition[2].name = kFieldName2;
    kExpectredDefinition[2].type = DT_BOOLEAN;
    kExpectredDefinition[3].name = kFieldName3;
    kExpectredDefinition[3].type = DT_STRING;

    auto& pLayer = pDataset->createGeorefMetadataLayer(keyType, UNKNOWN_METADATA_PROFILE, layerName,
        kExpectredDefinition, chunkSize, compressionLevel);

    const auto& valueTable = pLayer.getValueTable();

    UNSCOPED_INFO("Check getting the definition returns expected definition.");
    const auto& definition = valueTable.getDefinition();
    CHECK(definition.size() == kExpectedDefinitionSize);
    CHECK(definition[0] == kExpectredDefinition[0]);
    CHECK(definition[1] == kExpectredDefinition[1]);
    CHECK(definition[2] == kExpectredDefinition[2]);
    CHECK(definition[3] == kExpectredDefinition[3]);

    UNSCOPED_INFO("Check that there are no user defined records are in the value table (just the no data value record).");
    CHECK(valueTable.getRecords().size() == 1);

    UNSCOPED_INFO("Check getting the field index returns the expected result.");
    CHECK(valueTable.getFieldIndex(kFieldName0) == 0);
    CHECK(valueTable.getFieldIndex(kFieldName1) == 1);
    CHECK(valueTable.getFieldIndex(kFieldName2) == 2);
    CHECK(valueTable.getFieldIndex(kFieldName3) == 3);

    UNSCOPED_INFO("Check getting the field name returns the expected result.");
    CHECK(std::string{valueTable.getFieldName(0)} == std::string{kFieldName0});
    CHECK(std::string{valueTable.getFieldName(1)} == std::string{kFieldName1});
    CHECK(std::string{valueTable.getFieldName(2)} == std::string{kFieldName2});
    CHECK(std::string{valueTable.getFieldName(3)} == std::string{kFieldName3});
}

//  void addRecord(const Record& record);
//  const Records& getRecords() const & noexcept;
//  const CompoundDataType& getValue(size_t recordIndex, size_t fieldIndex) const &;
//  void setValue(size_t recordIndex, size_t fieldIndex, const CompoundDataType& value);
TEST_CASE("test value table add record", "[valuetable][constructor][addRecord][getRecords][getValue][setValue]")
{
    const TestUtils::RandomFileGuard tmpFileName;
    const std::string kExpectedLayerName = "uncertainty";
    constexpr size_t kExpectedNumRecords = 2;

    BAG::Record kExpectedNewRecord0 {
        BAG::CompoundDataType{std::string{"Bob"}},
        BAG::CompoundDataType{std::string{"Jones"}},
        BAG::CompoundDataType{42.2f},
        BAG::CompoundDataType{102u},
        BAG::CompoundDataType{true},
        BAG::CompoundDataType{1234.567f},
        BAG::CompoundDataType{std::string{"101 Tape Drive"}}
    };

    // Write a record.
    {
        BAG::Metadata metadata;
        metadata.loadFromBuffer(kMetadataXML);

        constexpr uint64_t chunkSize = 100;
        constexpr int compressionLevel = 6;

        auto pDataset = Dataset::create(tmpFileName, std::move(metadata),
            chunkSize, compressionLevel);
        REQUIRE(pDataset);

        // Make a new Value Table.
        constexpr BAG::DataType keyType = DT_UINT16;

        BAG::RecordDefinition definition(7);
        definition[0].name = "first name";
        definition[0].type = DT_STRING;
        definition[1].name = "last name";
        definition[1].type = DT_STRING;
        definition[2].name = "float value0";
        definition[2].type = DT_FLOAT32;
        definition[3].name = "bool value";
        definition[3].type = DT_UINT32;
        definition[4].name = "uint32 value";
        definition[4].type = DT_BOOLEAN;
        definition[5].name = "float value1";
        definition[5].type = DT_FLOAT32;
        definition[6].name = "address";
        definition[6].type = DT_STRING;

        auto& pLayer = pDataset->createGeorefMetadataLayer(keyType, UNKNOWN_METADATA_PROFILE,
            kExpectedLayerName, definition, chunkSize, compressionLevel);

        auto& valueTable = pLayer.getValueTable();

        UNSCOPED_INFO("Check that there are no user defined records are in the value table (just the no data value record).");
        CHECK(valueTable.getRecords().size() == 1);

        UNSCOPED_INFO("Adding a valid record to the value table does not throw.");
        REQUIRE_NOTHROW(valueTable.addRecord(kExpectedNewRecord0));

        UNSCOPED_INFO("Check there is one user defined record in the value table.");
        CHECK(valueTable.getRecords().size() == kExpectedNumRecords);

        UNSCOPED_INFO("Check adding an invalid record to the value table throws.");
        BAG::Record kJunkRecord;
        REQUIRE_THROWS(valueTable.addRecord(kJunkRecord));

        UNSCOPED_INFO("Check there is still one user defined record in the value table.");
        CHECK(valueTable.getRecords().size() == kExpectedNumRecords);
    }

    // Read the new record.
    {
        const auto pDataset = Dataset::open(tmpFileName, BAG_OPEN_READONLY);
        REQUIRE(pDataset);

        const auto layer = pDataset->getGeorefMetadataLayer(kExpectedLayerName);
        REQUIRE(layer);

        const auto& valueTable = layer->getValueTable();
        const auto& records = valueTable.getRecords();
        CHECK(records.size() == kExpectedNumRecords);

        constexpr size_t kKey = 1;
        size_t fieldIndex = 0;

        const auto& field0value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field0value == kExpectedNewRecord0[fieldIndex]);
        ++fieldIndex;

        const auto& field1value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field1value == kExpectedNewRecord0[fieldIndex]);
        ++fieldIndex;

        const auto& field2value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field2value == kExpectedNewRecord0[fieldIndex]);
        ++fieldIndex;

        const auto& field3value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field3value == kExpectedNewRecord0[fieldIndex]);
        ++fieldIndex;

        const auto& field4value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field4value == kExpectedNewRecord0[fieldIndex]);
        ++fieldIndex;

        const auto& field5value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field5value == kExpectedNewRecord0[fieldIndex]);
        ++fieldIndex;

        const auto& field6value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field6value == kExpectedNewRecord0[fieldIndex]);
        ++fieldIndex;

        REQUIRE_THROWS(valueTable.getValue(kKey, fieldIndex));
    }

    BAG::Record kExpectedNewRecord1 {
        BAG::CompoundDataType{std::string{"Ernie"}},
        BAG::CompoundDataType{std::string{"Jones"}},
        BAG::CompoundDataType{987.6543f},
        BAG::CompoundDataType{1001u},
        BAG::CompoundDataType{false},
        BAG::CompoundDataType{0.08642f},
        BAG::CompoundDataType{std::string{"404 Disk Drive"}}
    };

    // Set some new values an existing record.
    {
        auto pDataset = Dataset::open(tmpFileName, BAG_OPEN_READ_WRITE);
        REQUIRE(pDataset);

        auto layer = pDataset->getGeorefMetadataLayer(kExpectedLayerName);
        REQUIRE(layer);

        auto& valueTable = layer->getValueTable();

        const auto& records = valueTable.getRecords();
        CHECK(records.size() == kExpectedNumRecords);

        constexpr size_t kKey = 1;
        size_t fieldIndex = 0;

        // Read values back from memory.
        valueTable.setValue(kKey, fieldIndex, kExpectedNewRecord1[fieldIndex]);

        const auto& field0value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field0value == kExpectedNewRecord1[fieldIndex]);

        ++fieldIndex;
        valueTable.setValue(kKey, fieldIndex, kExpectedNewRecord1[fieldIndex]);

        const auto& field1value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field1value == kExpectedNewRecord1[fieldIndex]);

        ++fieldIndex;
        valueTable.setValue(kKey, fieldIndex, kExpectedNewRecord1[fieldIndex]);

        const auto& field2value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field2value == kExpectedNewRecord1[fieldIndex]);

        ++fieldIndex;
        valueTable.setValue(kKey, fieldIndex, kExpectedNewRecord1[fieldIndex]);

        const auto& field3value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field3value == kExpectedNewRecord1[fieldIndex]);

        ++fieldIndex;
        valueTable.setValue(kKey, fieldIndex, kExpectedNewRecord1[fieldIndex]);

        const auto& field4value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field4value == kExpectedNewRecord1[fieldIndex]);

        ++fieldIndex;
        valueTable.setValue(kKey, fieldIndex, kExpectedNewRecord1[fieldIndex]);

        const auto& field5value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field5value == kExpectedNewRecord1[fieldIndex]);

        ++fieldIndex;
        valueTable.setValue(kKey, fieldIndex, kExpectedNewRecord1[fieldIndex]);

        const auto& field6value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field6value == kExpectedNewRecord1[fieldIndex]);
    }

    // Read new values back from the HDF5 file.
    {
        const auto pDataset = Dataset::open(tmpFileName, BAG_OPEN_READONLY);
        REQUIRE(pDataset);

        const auto layer = pDataset->getGeorefMetadataLayer(kExpectedLayerName);
        REQUIRE(layer);

        const auto& valueTable = layer->getValueTable();

        const auto& records = valueTable.getRecords();
        CHECK(records.size() == kExpectedNumRecords);

        constexpr size_t kKey = 1;
        size_t fieldIndex = 0;

        const auto& field0value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field0value == kExpectedNewRecord1[fieldIndex]);
        ++fieldIndex;

        const auto& field1value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field1value == kExpectedNewRecord1[fieldIndex]);
        ++fieldIndex;

        const auto& field2value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field2value == kExpectedNewRecord1[fieldIndex]);
        ++fieldIndex;

        const auto& field3value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field3value == kExpectedNewRecord1[fieldIndex]);
        ++fieldIndex;

        const auto& field4value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field4value == kExpectedNewRecord1[fieldIndex]);
        ++fieldIndex;

        const auto& field5value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field5value == kExpectedNewRecord1[fieldIndex]);
        ++fieldIndex;

        const auto& field6value = valueTable.getValue(kKey, fieldIndex);
        CHECK(field6value == kExpectedNewRecord1[fieldIndex]);
        ++fieldIndex;

        REQUIRE_THROWS(valueTable.getValue(kKey, fieldIndex));
    }
}

//  void addRecords(const Records& records);
TEST_CASE("test value table add records", "[valuetable][constructor][addRecords][getRecords]")
{
    const TestUtils::RandomFileGuard tmpFileName;
    const std::string kExpectedLayerName = "elevation";

    // The Record definition.
    BAG::Record kExpectedNewRecord0 {
        BAG::CompoundDataType{false},
        BAG::CompoundDataType{std::string{"string1"}},
        BAG::CompoundDataType{true}
    };

    // The expected Records.
    using BAG::CompoundDataType;
    const BAG::Records kExpectedRecords {
        {CompoundDataType{true}, CompoundDataType{std::string{"string 1"}}, CompoundDataType{true}},
        {CompoundDataType{true}, CompoundDataType{std::string{"string 2"}}, CompoundDataType{false}},
        {CompoundDataType{false}, CompoundDataType{std::string{"string 3"}}, CompoundDataType{true}},
        {CompoundDataType{false}, CompoundDataType{std::string{"string 4"}}, CompoundDataType{false}}
    };

    const size_t kExpectedNumRecords = kExpectedRecords.size() + 1;

    // Write a record.
    {
        BAG::Metadata metadata;
        metadata.loadFromBuffer(kMetadataXML);

        constexpr uint64_t chunkSize = 100;
        constexpr int compressionLevel = 6;

        auto pDataset = Dataset::create(tmpFileName, std::move(metadata),
            chunkSize, compressionLevel);
        REQUIRE(pDataset);

        // Make a new Value Table.
        constexpr BAG::DataType keyType = DT_UINT16;

        BAG::RecordDefinition definition;
        definition.resize(3);
        definition[0].name = "bool1";
        definition[0].type = DT_BOOLEAN;
        definition[1].name = "string";
        definition[1].type = DT_STRING;
        definition[2].name = "bool2";
        definition[2].type = DT_BOOLEAN;

        auto& layer = pDataset->createGeorefMetadataLayer(keyType, UNKNOWN_METADATA_PROFILE,
            kExpectedLayerName, definition, chunkSize, compressionLevel);

        UNSCOPED_INFO("Check writing to variable resolution metadata fails because the dataset does not have variable resolution.");
        const uint32_t indexStart = 0;
        const uint32_t indexEnd = 0;
        const uint8_t buffer = 42;
        REQUIRE_THROWS(layer.writeVR(indexStart, indexEnd, &buffer));

        auto& valueTable = layer.getValueTable();

        UNSCOPED_INFO("Check that there are no user defined records are in the value table (just the no data value record).");
        CHECK(valueTable.getRecords().size() == 1);

        UNSCOPED_INFO("Adding a valid record to the value table does not throw.");
        REQUIRE_NOTHROW(valueTable.addRecords(kExpectedRecords));

        UNSCOPED_INFO("Check there are four records in the value table.");
        CHECK(valueTable.getRecords().size() == kExpectedNumRecords);
    }

    // Read the new records.
    {
        const auto pDataset = Dataset::open(tmpFileName, BAG_OPEN_READONLY);
        REQUIRE(pDataset);

        const auto pLayer = pDataset->getGeorefMetadataLayer(kExpectedLayerName);
        REQUIRE(pLayer);

        UNSCOPED_INFO("Check reading from variable resolution metadata fails because the dataset does not have variable resolution.");
        const uint32_t indexStart = 0;
        const uint32_t indexEnd = 0;
        REQUIRE_THROWS(pLayer->readVR(indexStart, indexEnd));

        const auto& valueTable = pLayer->getValueTable();
        const auto& records = valueTable.getRecords();  // Contains the no data value record.
        CHECK(records.size() == kExpectedNumRecords);

        // Read a record and make sure the values are expected.
        constexpr size_t kKey = 2;  // Get the second record value.

        {
            constexpr size_t fieldIndex = 1;  // Second field (string).

            const auto& value = valueTable.getValue(kKey, fieldIndex);
            CHECK(value == kExpectedRecords[kKey - 1][fieldIndex]);
            CHECK(value == records[kKey][fieldIndex]);
        }

        {
            std::string fieldName{"string"};  // Field called "string".
            const size_t kExpectedFieldIndex = 1;

            const auto fieldIndex = valueTable.getFieldIndex(fieldName);
            CHECK(fieldIndex == kExpectedFieldIndex);

            const auto& value = valueTable.getValue(kKey, fieldName);
            CHECK(value == kExpectedRecords[kKey - 1][fieldIndex]);
            CHECK(value == records[kKey][fieldIndex]);
        }
    }
}

//  UInt8Array readVR(uint32_t indexStart, uint32_t indexEnd) const;
//  void writeVR(uint32_t indexStart, uint32_t indexEnd, const uint8_t* buffer);
TEST_CASE("test georeferenced metadata layer readVR/writeVR", "[georefMetadatalayer][readvr][writevr]")
{
    const TestUtils::RandomFileGuard tmpFileName;
    const std::string kExpectedLayerName = "elevation";

    // Expected keys in the single resolution DataSet.
    const size_t kExpectedNumSRKeys = 3;
    std::array<uint8_t, kExpectedNumSRKeys> kExpectedSingleResolutionKeys{1, 2, 3};

    // Expected keys in the variable resolution DataSet.
    const size_t kExpectedNumVRKeys = 5;
    std::array<uint8_t, kExpectedNumVRKeys> kExpectedVariableResolutionKeys{3, 3, 2, 1, 1};

    // Create a dataset, enable VR, write then read some variable resolution spatial metadata.
    {
        BAG::Metadata metadata;
        metadata.loadFromBuffer(kMetadataXML);

        constexpr uint64_t chunkSize = 100;
        constexpr int compressionLevel = 6;

        auto pDataset = Dataset::create(tmpFileName, std::move(metadata),
            chunkSize, compressionLevel);
        REQUIRE(pDataset);

        // Enable variable resolution for the dataset.
        constexpr bool kMakeNode = false;
        pDataset->createVR(chunkSize, compressionLevel, kMakeNode);

        // Make a new georeferenced metadata layer using the elevation layer.
        // Key type is an 8 bit integer.  Valid values are 1-255.  0 is "no value".
        constexpr BAG::DataType keyType = DT_UINT8;

        // The metadata definition is:
        //      field name      field type
        //      "bool1"         boolean
        //      "string"        string
        //      "bool2"         boolean
        BAG::RecordDefinition definition;
        definition.resize(3);
        definition[0].name = "bool1";
        definition[0].type = DT_BOOLEAN;
        definition[1].name = "string";
        definition[1].type = DT_STRING;
        definition[2].name = "bool2";
        definition[2].type = DT_BOOLEAN;

        auto& layer = pDataset->createGeorefMetadataLayer(keyType, UNKNOWN_METADATA_PROFILE,
            kExpectedLayerName, definition, chunkSize, compressionLevel);

        // Write some single resolution metadata.
        // Write keys 1, 2, 3 to row 0, column 0-2 (inclusive).
        constexpr uint32_t rowStart = 0;
        constexpr uint32_t columnStart = 0;
        constexpr uint32_t rowEnd = 0;
        constexpr uint32_t columnEnd = 2;
        layer.write(rowStart, columnStart, rowEnd, columnEnd, kExpectedSingleResolutionKeys.data());

        // Write some variable resolution metadata.
        // Write keys 3, 3, 2, 1, 1 to indices 100-104 (inclusive).
        constexpr uint32_t indexStart = 100;
        constexpr uint32_t indexEnd = 104;
        layer.writeVR(indexStart, indexEnd, kExpectedVariableResolutionKeys.data());
    }

    // Load the dataset, and Read keys from the single and variable resolution
    // DataSets in the elevation georeferenced metadata layer.
    const auto pDataset = Dataset::open(tmpFileName, BAG_OPEN_READONLY);
    REQUIRE(pDataset);

    const auto pLayer = pDataset->getGeorefMetadataLayer(kExpectedLayerName);
    REQUIRE(pLayer);

    // Read single resolution metadata keys.
    {
        // Select row 0, and columns 0-2 (3 values total).
        constexpr uint32_t rowStart = 0;
        constexpr uint32_t columnStart = 0;
        constexpr uint32_t rowEnd = 0;
        constexpr uint32_t columnEnd = 2;
        const auto buffer = pLayer->read(rowStart, columnStart, rowEnd, columnEnd);

        UNSCOPED_INFO("Check the single resolution metadata keys are expected.");
        CHECK(buffer.size() == kExpectedNumSRKeys);

        for (size_t i=0; i<kExpectedNumSRKeys; ++i)
            CHECK(buffer[i] == kExpectedSingleResolutionKeys[i]);
    }

    // Read variable resolution metadata keys.
    {
        // Read from index 100-104 (5 values total).
        constexpr uint32_t indexStart = 100;
        constexpr uint32_t indexEnd = 104;
        const auto buffer = pLayer->readVR(indexStart, indexEnd);

        UNSCOPED_INFO("Check the variable resolution metadata keys are expected.");
        CHECK(buffer.size() == kExpectedNumVRKeys);

        for (size_t i=0; i<kExpectedNumVRKeys; ++i)
            CHECK(buffer[i] == kExpectedVariableResolutionKeys[i]);
    }
}


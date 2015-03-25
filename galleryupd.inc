<?php
/**
* @file
* Migration module to associate images CT to Gallery content type.
* Carlos A. Godinez, Principal CMS Engineer
* Mon Feb  9 09:13:58 EST 2015
*/
 
class GalleryUpdMigration extends XMLMigration {
	public function __construct($arguments) {
	parent::__construct($arguments);
        $this->description = t('InStyle Gallery Content Type Migration Update');

        $fields = array(
            'field_legacy_id' => t('Vignette component_id'),
            'field_images' => t('Vignette images'),
            'field_slides' => t('Slides grouping; populated by PrepareRow'),
        );

        $this->map = new MigrateSQLMap($this->machineName,
            array(
                'field_legacy_id' => array(
                'type' => 'int',
                'not null' => TRUE,
                ),
            ),
            MigrateDestinationNode::getKeySchema()
        );

        // XML File
        $xml_folder = DRUPAL_ROOT . '/sites/default/files/migration/vignette/galleryupd';
        $files = file_scan_directory($xml_folder, '/\.xml$/');
        $items_url = array_keys($files);

        $item_xpath = '/root/gallery';  // relative to document
        $item_ID_xpath = 'fields/field_legacy_id';         // relative to item_xpath

        $source_options = array(
            'cache_counts' => TRUE,
        );

        // Source and destination
        $this->systemOfRecord = Migration::DESTINATION;
        $this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
        $this->destination = new MigrateDestinationNode('gallery');

        // Mapped fields
        $this->addFieldMapping('nid', 'field_legacy_id')
            ->sourceMigration('Gallery');

        $this->addFieldMapping('field_slides', 'field_slides')
            ->xpath('field_slides')
            ->separator(',')
            ->sourceMigration('ImageComponent')
            ->arguments(array('source_type' => 'nid'));

        // Unmapped source fields
                                                                                                                                                                   
        // Unmapped destination fields
        $this->addFieldMapping('title');
        $this->addFieldMapping('field_headline');
        $this->addFieldMapping('metatag_description');
        $this->addFieldMapping('field_description');
        $this->addFieldMapping('field_ad_zone');
        $this->addFieldMapping('field_category');
        $this->addFieldMapping('field_subcategory');
        $this->addFieldMapping('field_gallery_type');
        $this->addFieldMapping('field_display_ads');
        $this->addFieldMapping('field_legacy_url');
        $this->addFieldMapping('created');
        $this->addFieldMapping('changed');
        $this->addFieldMapping('field_not_searchable');
    }

    public function prepareRow($row) {

        // Build slides
        $field_slides = array();
        foreach ($row->xml->components->field_images->field_image as $field_image) {
            $field_slides[] = (string) $field_image;
        }

        // Concat slides legacy-ids into one string
        $row->xml->field_slides = implode(",", $field_slides);

        //drush_print_r($row);
        return TRUE;
    }
}

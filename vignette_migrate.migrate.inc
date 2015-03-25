<?php
/**
 * @file
 * Describe the Vignette migration to the Migrate API.
 */

/**
 * Implements hook_migrate_api().
 */
function vignette_migrate_migrate_api() {
  $api = array(
    'api' => 2,
    'field handlers' => array(
      'DateMigrateFieldHandler',
    ),
    'groups' => array(
      'InStyle' => array(
        'title' => 'InStyle',
      ),
	),
    'migrations' => array(
      'ImageComponent' => array(
        'class_name' => 'ImageComponentMigration',
	  	'group_name' => 'InStyle',
      ),
      'BBBProduct' => array(
        'class_name' => 'bbbProductMigration',
	  	'group_name' => 'InStyle',
      ),
      'GalleryUpd' => array(
        'class_name' => 'GalleryUpdMigration',
	  	'group_name' => 'InStyle',
      ),
      'Gallery' => array(
        'class_name' => 'GalleryMigration',
	  	'group_name' => 'InStyle',
      ),
      'LookOfTheDay' => array(
        'class_name' => 'LookOfTheDayMigration',
	  	'group_name' => 'InStyle',
      ),
      'Tout' => array(
        'class_name' => 'ToutMigration',
	  	'group_name' => 'InStyle',
      ),
      'PackageHub' => array(
        'class_name' => 'PackageHubMigration',
	  	'group_name' => 'InStyle',
      ),
      'Package' => array(
        'class_name' => 'PackageMigration',
	  	'group_name' => 'InStyle',
      ),
      'Video' => array(
        'class_name' => 'VideoMigration',
	  	'group_name' => 'InStyle',
	  ),
      'FileImageCopy' => array(
        'class_name' => 'ImageFileCopyMigration',
	  	'group_name' => 'InStyle',
	  ),
    ),
  );
  return $api;
}

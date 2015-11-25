<?php

/**
 * @file
 * Contains \Drupal\message\MessageTypeListBuilder.
 */

namespace Drupal\message;

use Drupal\Core\Config\Entity\ConfigEntityListBuilder;
use Drupal\Component\Utility\Xss;

/**
 * Defines a class to build a listing of message type entities.
 *
 * @see \Drupal\message\Entity\MessageType
 */
class MessageTypeListBuilder extends ConfigEntityListBuilder {

  /**
   * {@inheritdoc}
   */
  public function buildHeader() {
    $header['title'] = t('Name');
    $header['description'] = array(
      'data' => t('Description'),
      'class' => array(RESPONSIVE_PRIORITY_MEDIUM),
    );
    return $header + parent::buildHeader();
  }

  /**
   * {@inheritdoc}
   */
  public function buildRow(MessageTypeInterface $entity) {
    $row['title'] = array(
      'data' => $entity->label(),
      'class' => array('menu-label'),
    );

    /* @var $entity \Drupal\message\Entity\MessageType */
    $row['description'] = Xss::filterAdmin($entity->getDescription());
    return $row + parent::buildRow($entity);
  }

}
